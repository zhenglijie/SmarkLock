/**
 * @file 完成版.CPP
 * @brief 通过WiFi与服务器通信;获取按键
 * @version 1.8
 * @date 2022-06-30
 * @author
 *
 * 1.5改动
 * 1.修改了发送给服务器的字符串ARPS+ID+_密码
 * 2.在复位时发送ID
 *
 * 1.6改动
 * 1.加入输错指定次数（5）禁止使用6s
 *
 * 1.7改动
 * 1.加入分级输错持续锁定功能
 *
 * 1.8 xSP改动
 * 1.加入离线默认密码和开锁功能
 * 2.加入开锁修改密码功能
 *
 * 1.9改动
 * 1.加入使用计数器Aticker每20s尝试连接服务器功能
 *
 * 2.0改动
 * 1.将密码放入Flash文件系统中操作，掉电不消失
 *
 * 使用前需修改 COM号 波特率 wifi名-密码 服务器ip地址
 *
 */
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Keypad.h>
#include <String>
#include <Ticker.h>
#include <FS.h>

#define STASSID "zxc"       // wifi名称
#define PASSWD "zlj1234567" // wifi密码

uint32_t chipid; //芯片ID
unsigned char countL = 0;
unsigned char flagL = 0;
unsigned char readFlag = 0;
unsigned char writeFlag = 0;

String file_name = "/taichi-maker/notes.txt"; //被读取的文件位置和名称
String codeBuf = "123456";                    //暂时存放密码

const char *host = "192.168.43.63"; // 网络服务器地址
const int httpPort = 8891;          // http端口80

const byte ROWS = 4;      // four rows
const byte COLS = 4;      // four columns
char keys[ROWS][COLS] = { //键盘键值
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {D7, D6, D5, D4}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {D3, D2, D1, D0}; // connect to the column pinouts of the keypad
String dPasswd = "123456";             //默认密码

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
WiFiClient client;
String Input;
Ticker ticker;
Ticker Aticker;                   //连接服务器定时
unsigned char AtickerCounter = 0; //连服务器定时器
char key;

void setup()
{
    chipid = ESP.getChipId() % 1000000;

    Serial.begin(115200); //以115200波特率来打开串口
    Serial.println("connect successfully!");

    WiFi.mode(WIFI_STA); //设置为客户端模式

    pinMode(D8, OUTPUT); //引脚D8设置为输出模式

    //开始连接wifi
    WiFi.begin(STASSID, PASSWD);
    int WiFiCount = 0;
    int WiFiFlag = 0; // WiFi连接失败置1，成功为0
    //等待WiFi连接,连接成功打印IP

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        WiFiCount++;
        Serial.print(".");
        if (WiFiCount >= 15)
        {
            Serial.println("Connect WiFi Time Out!");
            WiFiFlag = 1;
            break;
        }
    }
    if (WiFiFlag == 0)
    {
        Serial.println("");
        Serial.println("WiFi Connected!");
    }

    dPasswd = myReadFlash(); //从FS读取密码
    Serial.println("Passwd is " + dPasswd);
    client.print(chipid);
    Serial.printf("chipid: %d", chipid);
    ticker.attach(0.05, unLock); //中断，每0.05秒执行一次unLock函数
    Aticker.attach(1, AtickerCount);
    wifiClientRequest();

    // sendInfo();//发送ID信息
}

void AtickerCount()
{
    AtickerCounter++;
}

void unLock()
{

    if (flagL == 1) //输错密码达到一定次数锁定，禁止操作一定时间
    {               //锁3s
        Serial.println("You cannot input until 3s");
        if (countL++ >= 60)
        {
            countL = 0;
            flagL = 0;
            Serial.println("You can input combination");
            key = keypad.getKey();
        }
        else
        {
            return;
        }
    }
    else if (flagL == 2)
    { //锁6s
        Serial.println("You cannot input until 6s");
        if (countL++ >= 120)
        {
            countL = 0;
            flagL = 0;
            Serial.println("You can input combination");
            key = keypad.getKey();
        }
    }
    else if (flagL == 3)
    { //锁9s
        Serial.println("You cannot input until 9s");
        if (countL++ >= 180)
        {
            countL = 0;
            flagL = 0;
            Serial.println("You can input combination");
            key = keypad.getKey();
        }
    }
    else if (flagL == 4)
    { //永久锁定
        Serial.println("Please contact administrator for assistance.");
        return;
    }
    else
    {
        key = keypad.getKey();
    }

    // key = keypad.getKey(); //获取键值
    //   if(idCount++>=60)//每3s向服务器发送一次ID
    //   {
    //     idCount=0;
    //     sendInfo();
    //   }

    if (key != NO_KEY)
    {
        int idx;
        String recv;
        int WiFiCount = 0;
        int WiFiFlag = 0; // WiFi连接失败置1，成功为0
        Serial.print("you input key = ");
        Serial.println(key);
        switch (key)
        {
            /*case 'A'://联网
                //开始连接wifi
                WiFi.begin(STASSID, PASSWD);

                //等待WiFi连接,连接成功打印IP

                while (WiFi.status() != WL_CONNECTED)
                {
                    delay(1000);
                    WiFiCount++;
                    Serial.println(".");
                    if(WiFiCount>=10)
                    {
                      Serial.println("Reconnect WiFi Time Out!");
                      WiFiFlag=1;
                      break;
                    }
                }
                if(WiFiFlag==0)
                {
                  Serial.println("");
                  Serial.println("WiFi Connected!");
                  wifiClientRequest();
                }
                break;*/

        case 'C': // SimLock
            digitalWrite(15, LOW);
            client.print("ARID" + String(chipid) + "_LOCK");
            break;
        case '*': // delete
            idx = (int)Input.length() - 1;
            if (idx < 0)
            {
                Serial.println("delete error!");
            }
            else
            {
                char c = Input.charAt(idx);
                Serial.print("delete:");
                Serial.println(c);
                Input.remove(idx);
                Serial.println("cur Input is " + Input);
            }
            break;
        case '#': // submit
            if (Input.length() < 6)
            {
                Serial.println("the length of yout input < 6");
                break;
            }
            Serial.println("cur passwd is " + dPasswd);
            if (dPasswd == Input)
            {
                digitalWrite(15, HIGH);
            }

            Serial.println("unlock?");
            client.print("ARPS" + String(chipid) + "_" + Input);
            Serial.println("ARPS" + String(chipid) + "_" + Input);
            Input.clear();

            break;
        case 'D': // reset combination
            if (digitalRead(15))
            {
                if (Input.length() < 6)
                {
                    writeFlag = 1;
                    readFlag = 1;
                    //
                    Serial.println("the length of yout input < 6");
                    break;
                }
                else
                {
                    writeFlag = 1;
                    readFlag = 1;
                    codeBuf = Input;
                    Serial.printf("%d %d\n", readFlag, writeFlag);
                    // dPasswd=Input;
                    Serial.println("Rest Combination Success");
                    Input.clear();
                    // break;
                }
            }
            // break;
        default:
            if (key >= '0' && key <= '9')
            {
                if (Input.length() >= 6)
                {
                    Serial.println("the length of yout input = 6");
                    Serial.println("Combination is already enough!");
                    break;
                }
                Input += key;
                Serial.println("cur Input is " + Input);
            }
        }
    }

    // Serial.println(writeFlag);
    if (readFlag == 1)
    {
        dPasswd = myReadFlash();

        readFlag = 0;
    }
    if (writeFlag == 1)
    {
        dPasswd = codeBuf;
        Serial.println("after modify passwd is " + dPasswd);
        myWriteFlash();
        writeFlag = 0;
    }
}

void sendInfo()
{
    Serial.println(chipid);
    client.print(chipid);
}

void wifiClientRequest()
{
    // 建立WiFi客户端对象，对象名称client

    // 建立字符串，用于HTTP请求
    //    String httpRequest = String("GET /") + " HTTP/1.1\r\n" +
    //                         "Host: " + host + "\r\n" +
    //                         "Connection: close\r\n" +
    //                         "\r\n";

    // 通过串口输出连接服务器名称以便查阅连接服务器的网址
    Serial.print("Connecting to ");
    Serial.println(host);

    if (client.connect(host, httpPort))
    {
        Serial.println("Success!"); // 连接成功后串口输出“Success”信息
        // client.println("hello Server!");

        client.println("ARID" + String(chipid) + "_LOGIN");

        //        client.print(httpRequest);           // 向服务器发送HTTP请求
        Serial.println("Sending request: "); // 通过串口输出HTTP请求信息内容以便查阅
                                             //        Serial.println(httpRequest);

        Serial.println("Web Server Response: ");
        while (client.connected() || client.available())
        {
            if (client.available())
            {
                String line = client.readStringUntil('\n');
                Serial.println(line);
                if (line[0] == '1')
                {
                    digitalWrite(15, HIGH);
                }
                /**********   判断输错密码信号    **********/
                if (line[0] == 'W')
                {
                    flagL = 1;
                }
                else if (line[0] == 'X')
                {
                    flagL = 2;
                }
                else if (line[0] == 'Y')
                {
                    flagL = 3;
                }
                else if (line[0] == 'Z')
                {
                    flagL = 4;
                }
            }
        }

        client.stop();                      // 断开与服务器的连接
        Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
        Serial.print(host);
    }
    else
    { // 如果连接不成功则通过串口输出“连接失败”信息
        Serial.println("connection failed!");
        client.stop();
    }
}

void myWriteFlash() //自用封装 写入Flash文件系统文件函数
{
    if (SPIFFS.begin())
    { // 启动闪存文件系统
        Serial.println("SPIFFS Started.");
    }
    else
    {
        Serial.println("SPIFFS Failed to Start.");
    }

    File wdataFile = SPIFFS.open(file_name, "w"); // 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
    Serial.println("writeFlash:" + codeBuf);
    wdataFile.println(codeBuf); // 向dataFile写入字符串信息
    wdataFile.close();          // 完成文件写入后关闭文件
    Serial.println("Finished Writing data to SPIFFS");
    SPIFFS.end();
    Serial.println("SPIFFS Ended.");
}

String myReadFlash() //自用封装 读取Flash文件系统文件函数
{
    if (SPIFFS.begin())
    { // 启动闪存文件系统
        Serial.println("SPIFFS Started.");
    }
    else
    {
        Serial.println("SPIFFS Failed to Start.");
    }

    //确认闪存中是否有file_name文件
    if (SPIFFS.exists(file_name))
    {
        Serial.print(file_name);
        Serial.println(" FOUND.");
    }
    else
    {
        Serial.print(file_name);
        Serial.print(" NOT FOUND.");
    }

    //建立File对象用于从SPIFFS中读取文件
    File rdataFile = SPIFFS.open(file_name, "r");

    //读取文件内容并且通过串口监视器输出文件信息
    String res;
    for (int i = 0; i < rdataFile.size() - 2; i++)
    {
        res += (char)rdataFile.read();
    }

    Serial.print(res);
    //完成文件读取后关闭文件
    rdataFile.close();
    SPIFFS.end();
    Serial.println("SPIFFS Ended.");
    return res;
}

void loop()
{
    // key = keypad.getKey();
    // delay(100);
    if (AtickerCounter >= 20)
    { //可能有反复执行的BUG，待测试
        wifiClientRequest();
    }
    //    if(readFlag=1)
    //    {
    //      dPasswd=myReadFlash();
    //      readFlag=0;
    //    }
    //    if(writeFlag=1)
    //    {
    //      myWriteFlash();
    //      writeFlag=0;
    //    }
}
#ifndef _SQLCONNECT_H
#define _SQLCONNECT_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

MYSQL *connection;
const char *host = "127.0.0.1";
const char *user = "root";
const char *pwd = "123456";
const char *db_name = "Myproject";
char Query[200];

void SQLinit() {
	connection = mysql_init(NULL);
	if (connection != NULL) {
		perror("mysql_init\n");
	} else {
		printf("mysql init failed!\n");
		exit(0);
	}
	
	connection = mysql_real_connect(connection, host, user, pwd, db_name, 3306, NULL, 0);
	
	if (connection != NULL) {
		perror("mysql_connect\n");
	} else {
		printf("mysql connect failed!\n");
	}
}

bool queryID(char *TableName, int ID) {
	memset(Query, 0, sizeof Query);
	sprintf(Query, "SELECT * FROM '%s' WHERE id = %d", TableName, ID);
	if (!mysql_query(connection, Query)) {
		printf("Arduino %d have signed in\n", ID);
		return true;
	} else return false;
}

//if no exist ID, need Insert
void ArduinoSignIn(int ID, char *IP, int connfd) {
	memset(Query, 0, sizeof Query);
	
	sprintf(Query, "INSERT INTO arduinoInfo(arduinoID, arduinoIP, arduinoPasswd, arduinoconfd) VALUES(%d, '%s', '123456', %d)", ID, IP, connfd);
	if (mysql_query(connection, Query)) {
		printf("AR Insert Database failed!\n");
	} else {
		printf("AR Insert Database successfully!\n");
	}
} 

void QtSignIn(int ID, char *IP, int connfd) {
	memset(Query, 0, sizeof Query);
	
	sprintf(Query, "INSERT INTO qtInfo(QtID, QtIP, Qtconfd) VALUES(%d, '%s', %d)", ID, IP, connfd);
	if (mysql_query(connection, Query)) {
		printf("Qt Insert Database failed!\n");
	} else {
		printf("Qt Insert Database successfully!\n");
	}
} 

bool checkPasswd(int ID, char *Passwd) {
	//printf("ID = %d PS = %s\n", ID, Passwd);
	memset(Query, 0, sizeof Query);
	sprintf(Query, "SELECT arduinoPasswd FROM arduinoInfo WHERE arduinoID = %d", ID);
	
	if (!mysql_query(connection, Query)) {
		MYSQL_RES *result = mysql_store_result(connection);
		MYSQL_ROW row;
		unsigned int nums_fileds = mysql_num_fields(result);
		int i;
		if ((row = mysql_fetch_row(result))) {
			//printf("this passwd is %s\n", row[0]);
			//printf("\n");
			if (!strcmp(row[0], Passwd)) {
				mysql_free_result(result);
				//printf("unlock successfully\n");
				return true;
			} else {
				//printf("unlock unsuccessfully");
				mysql_free_result(result);
				return false;
			}
		}
	} else {
		printf("query failed!");
	}
	return false;
}

int getConfd(char *TableName, int ID) {
	memset(Query, 0, sizeof Query);
	if (!strcmp(TableName, "arduinoInfo")) {
		sprintf(Query, "SELECT arduinoconfd FROM arduinoInfo WHERE arduinoID = %d", ID);
	} else if (!strcmp(TableName, "qtInfo")) {
		sprintf(Query, "SELECT Qtconfd FROM qtInfo WHERE QtID = %d", ID);
	}
	
	if (!mysql_query(connection, Query)) {
		MYSQL_RES *result = mysql_store_result(connection);
		MYSQL_ROW row;
		unsigned int nums_fileds = mysql_num_fields(result);
		int i;
		if ((row = mysql_fetch_row(result))) {
			//printf("this Confd is %s\n", row[0]);
			//printf("\n");
			
			int cid = 0;
			for (int i = 0; i < strlen(row[0]); i++) {
				int x = (row[0][i] - '0');
				cid = cid * 10 + x;
			}
			return cid;
		}
	} else {
		printf("query failed!");
	}
	return -1;
}

void modifyconFd(char *TableName, int ID, int confd) {
	memset(Query, 0, sizeof Query);
	
	if (!strcmp(TableName, "arduinoInfo")) {
		sprintf(Query, "UPDATE arduinoInfo set arduinoconfd = %d WHERE arduinoID = %d", confd, ID);
	} else if (!strcmp(TableName, "qtInfo")) {
		sprintf(Query, "UPDATE qtInfo set Qtconfd = %d WHERE QtID = %d", confd, ID);
	}
	
	if (!mysql_query(connection, Query)) {
		printf("connectSocket modifyconFd successfully!\n");
	} else {
		printf("connectSocket modifyconFd unsuccessfully!\n");
	}
}

int Bind(int QTID, int ARID) {
	memset(Query, 0, sizeof Query);
	sprintf(Query, "SELECT ARID FROM qtInfo WHERE QtID = %d", QTID);
	bool havebind = 0;
	if (!mysql_query(connection, Query)) {
		MYSQL_RES *result = mysql_store_result(connection);
		MYSQL_ROW row;
		unsigned int nums_fileds = mysql_num_fields(result);
		int i;
		if ((row = mysql_fetch_row(result))) {
			//printf("this ARID is %s\n", row[0]);
			//printf("\n");
			int cid = 0;
			for (int i = 0; i < strlen(row[0]); i++) {
				int x = (row[0][i] - '0');
				cid = cid * 10 + x;
			}
			if (cid == ARID) { //have seek ARID successful. QT bind AR
				havebind = 1;
			}
		}
	} 
	
	
	
	if (havebind) {
		printf("Arduino %d and QT %d have binded\n", ARID, QTID);
		return 1;
	}
	
	bool flag = 0;
	memset(Query, 0, sizeof Query);
	sprintf(Query, "UPDATE qtInfo set ARID = %d WHERE QtID = %d", ARID, QTID);
	if (!mysql_query(connection, Query)) flag |= 1;
	
	memset(Query, 0, sizeof Query);
	sprintf(Query, "UPDATE arduinoInfo set appid = %d WHERE arduinoID = %d", QTID, ARID);
	if (!mysql_query(connection, Query)) flag |= 1;
	
	if (flag) {
		printf("bind between Arduino %d and Qt %d successfully!\n", ARID, QTID);
		return 2;
	} else {
		printf("bind between Arduino %d and Qt %d unsuccessfully!\n", ARID, QTID);
	}
	
	return 3; //bind failed!
}

int getQtID(int arid) {
	memset(Query, 0, sizeof Query);
	sprintf(Query, "SELECT appid FROM arduinoInfo WHERE arduinoID = %d", arid);
	bool havebind = 0;
	int appid = 0;
	if (!mysql_query(connection, Query)) {
		MYSQL_RES *result = mysql_store_result(connection);
		MYSQL_ROW row;
		unsigned int nums_fileds = mysql_num_fields(result);
		int i;
		if ((row = mysql_fetch_row(result))) {
			//printf("this appid is %s\n", row[0]);
			for (int i = 0; i < strlen(row[0]); i++) {
				int x = (row[0][i] - '0');
				appid = appid * 10 + x;
			}
		}
	} 
	
	return appid;
}

#endif



#include "Server.h"
#include "SQLconnect.h"
#include <stddef.h>

#define LISTENQ 25
#define MAXLINE 512

char sendbuff[MAXLINE], recvbuff[MAXLINE];
int listenfd;
socklen_t len;
struct sockaddr_in servaddr, cliaddr;
const int PORT = 8891; 
const char *IP = "192.168.43.63";
int idx = 0;
int cntlock = 0;
bool havelock = 1;

void newUser(void *t);

int setAddr() {
	memset(&servaddr, 0, sizeof servaddr);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = inet_addr(IP);
	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof servaddr) < 0) {
		return -1;
	}

	return 1;
}

int main() {
	SQLinit();
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
		printf("Socket creater failed.\n");
		return -1;
	}

	if (setAddr() == -1) {
		printf("bind failed.\n");
		return 0;
	}
	
	printf("Server listening...\n");
	listen(listenfd, LISTENQ);
	
	while (1) {
		len = sizeof(cliaddr);
		int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
		int sinport = cliaddr.sin_port;
		
		printf("\n\n\n");
		printf("connect from %s port %d\n", inet_ntoa(cliaddr.sin_addr), (int)ntohs(cliaddr.sin_port));
	
		if (connfd != -1) {
			pthread_t newThread;
			pthread_create(&newThread, NULL, (void *)&newUser, &connfd);
		}
	}

	return 0;
}

void newUser(void *t) {
	int retval;
	printf("Thread ID:%lx\n", pthread_self());
	int status;
	int connfd = *(int *)t;
	while (1) {	
		memset(recvbuff, 0, sizeof recvbuff);
		int n = recv(connfd, recvbuff, MAXLINE, 0);
		int len = strlen(recvbuff);
		if (!len) continue;
		printf("Server recv Info = %s\n", recvbuff);
		if (recvbuff[0] == 'A' && recvbuff[1] == 'R' && recvbuff[2] == 'I' && recvbuff[3] == 'D' && recvbuff[len - 1] == 'N') 			{ 	//Arduino online sign in  ARIDXXXXXX_LOGIN
			int ID = getId(recvbuff, 4);
			printf("\n--------Arduino %d SIGN IN--------\n", ID);
			
			if (queryID("arduinoInfo", ID)) {
				printf("Arduino %d have existed!\n", ID);
			} else {
				ArduinoSignIn(ID, (char *)inet_ntoa(cliaddr.sin_addr), connfd);
			}
			modifyconFd("arduinoInfo", ID, connfd);
			// first log, then sign in database,  
		} else if (recvbuff[0] == 'A' && recvbuff[1] == 'R' && recvbuff[2] == 'P' && recvbuff[3] == 'S') { //mi ma
			//ARPSXXXXXX_XXXXXX
			int ID = getId(recvbuff, 4);
			printf("\n--------Arduino %d UNLOCK--------\n", ID);
			int QtID = getQtID(ID);
			char *Passwd = getPasswd(recvbuff, 11);
			int QtFd = getConfd("qtInfo", QtID);
			//printf("QtFd is %d\n", QtFd);
			printf("Arduino %d input passwd is %s\n", ID, Passwd);
			if (checkPasswd(ID, Passwd)) {
				havelock = 0;
				send(connfd, "1", strlen("1"), 0); //Arduino light 
				send(QtFd, "SUCC", strlen("SUCC"), 0); //Qt show
				printf("Arduino %d unlock successfully\n", ID);
				cntlock = 0;
			} else {	
				if (!havelock) continue;
				cntlock++;
				printf("Arduino %d %d th unlock unsuccessfully\n", ID, cntlock);
				
				if (cntlock == 6) {
					printf("lock 3s\n");
					LOCK(3, connfd, QtFd);
				} else if (cntlock == 7) {
					printf("lock 6s\n");
					LOCK(6, connfd, QtFd);
				} else if (cntlock == 8) {
					printf("lock 9s\n");
					LOCK(9, connfd, QtFd);
				} else if (cntlock >= 9) {
					printf("lock forever\n");
					LOCK(10, connfd, QtFd);
				}
			}
		} else if (recvbuff[0] == 'Q' && recvbuff[1] == 'T' && recvbuff[2] == 'I' && recvbuff[3] == 'D' && recvbuff[len - 2] == 'N') { //check ID  QTID000001_LOGIN
			int ID = getId(recvbuff, 4);
			printf("\n--------QT %d LOGIN--------\n", ID);
			if (queryID("qtInfo", ID)) {
				printf("QTID %d have existed!\n", ID);
			} else {
				QtSignIn(ID, (char *)inet_ntoa(cliaddr.sin_addr), connfd);
			}
			send(connfd, "LOG_SUCCESS", strlen("LOG_SUCCESS"), 0);
			modifyconFd("qtInfo", ID, connfd);
		} else if (recvbuff[0] == 'A' && recvbuff[1] == 'R' && recvbuff[2] == 'I' && recvbuff[3] == 'D' && recvbuff[len - 1] == 'K') { //lock
			//ARIDXXXXXX_LOCK
			havelock = 1;
			int ID = getId(recvbuff, 4);
			printf("\n--------Arduino %d was Locked--------\n", ID);
			int QtID = getQtID(ID);
			int QtFd = getConfd("qtInfo", QtID);
			send(QtFd, "LOCK", strlen("LOCK"), 0);
		} else if (recvbuff[0] == 'Q' && recvbuff[1] == 'T' && recvbuff[2] == 'I' && recvbuff[3] == 'D' && recvbuff[11] == 'A' && recvbuff[12] == 'R') { 
			//bind QTIDXXXXXX_ARXXXXXX	
			int ID = getId(recvbuff, 4); //get self ID
			int bindNum = getBindNum(recvbuff, 13);
			printf("\n--------bind between Arduino %d and QT %d--------\n", bindNum, ID);
			int t = Bind(ID, bindNum);
			if (t == 1) {
				send(connfd, "havebind", strlen("havebind"), 0);
			} else if (t == 2) {
				send(connfd, "bindsucc", strlen("bindsucc"), 0);
			} else if (t == 3) {
				send(connfd, "bindfailed", strlen("bindfailed"), 0);
			}
		}
	}
	//wait(&status);
	//close(connfd);
	//close(listenfd);

	//pthread_exit(&retval);
}


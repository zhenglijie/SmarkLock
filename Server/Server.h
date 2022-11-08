#ifndef _SERVER_H
#define _SERVER_H

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <netdb.h>
#include <time.h>

int getId(char *s, int idx) {
	int len = strlen(s);
	int ID = 0, i;
	for (i = idx; i < idx + 6; i++) {
		int x = (s[i] - '0');
		ID = ID * 10 + x;
	}
	
	return ID;
}

char* getIP(char *s, int idx) {
	int len = strlen(s), i;
	static char IP[50];
	int cnt = 0;
	for (i = idx; i < len; i++) {
		IP[cnt++] = s[i];
	}
	
	IP[cnt] = '\0';
	return IP;
}

int getBindNum(char *s, int idx) {
	int ID = 0, i;
	for (i = idx; i < idx + 6; i++) {
		int x = (s[i] - '0');
		ID = ID * 10 + x;
	}
	
	return ID;
}

void LOCK(int TIME, int confd, int QtFd) {
	if (TIME == 3) {
		send(confd, "W", strlen("W"), 0);
		send(QtFd, "BANW", strlen("BANW"), 0);
	} else if (TIME == 6) {
		send(confd, "X", strlen("X"), 0);
		send(QtFd, "BANX", strlen("BANX"), 0);
	} else if (TIME == 9) {
		send(confd, "Y", strlen("Y"), 0);
		send(QtFd, "BANY", strlen("BANY"), 0);
	} else if (TIME == 10) {
		send(confd, "Z", strlen("Z"), 0);
		send(QtFd, "BANZ", strlen("BANZ"), 0);
	}
}

char* getPasswd(char *s, int idx) {
	int len = strlen(s);
	static char Passwd[50];
	int cnt = 0;
	for (int i = idx; i < len; i++) {
		Passwd[cnt++] = s[i];
	}
	
	Passwd[cnt] = '\0';
	return Passwd;
}

#endif

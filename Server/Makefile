server:server.o
	gcc server.o -o server
server.o:server.c Server.h SQLconnect.h
	gcc server.c Server.h SQLconnect.h -lmysqlclient -I/usr/include/mysql/ -L/usr/lib/mysql -pthread -o server.o

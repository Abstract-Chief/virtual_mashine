CC=gcc #varible
CCFLAGS= -g -Wall
LIB= -lncurses
SERV_ARGS= 
USER_ARGS= 127.0.0.1

%.o: %.c %.h
	$(CC) $(CCFLAGS) -c $<

server_comp: server/main.c server/internet.o server/module.o
	$(CC) $(CCFLAGS) server/main.c internet.o module.o -o build/server

clear:
	rm *.o

user_comp: user/internet.o
	$(CC) $(CCFLAGS) user/main.c internet.o -o build/user $(LIB)

server: server_comp clear
	build/server

user: user_comp clear
	build/user $(USER_ARGS)

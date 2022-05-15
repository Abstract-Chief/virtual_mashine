#ifndef STRUCTURE_H_SENTURY
#define STRUCTURE_H_SENTURY
#include<arpa/inet.h>

typedef struct{
   unsigned char maxfd;
   fd_set read,write;
   int count;
   unsigned char *data;
} Fd_talbe;

typedef struct{
   int y,x;
} Coord;

typedef struct{
   Coord cord;
   int ip;
} Player;

typedef struct{
   Player players[20];
   int count;
} Player_list;

#endif

#include"structure.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

void add_user(Player *user,struct sockaddr_in *addr){
   user->cord.x=5;
   user->cord.y=5;
   user->ip=addr->sin_addr.s_addr;
}

int move_player(Player *user,int route,int step){
   int x=1,y=0;
   switch(route){
      case(1):
         x=0,y=1;
         break;
      case(2):
         x=-1,y=0;
         break;
      case(3):
         x=0,y=-1;
         break;
   }
   user->cord.x+=x;
   user->cord.y+=y;
   return 0;
}

void delete_user(Player_list *pl_list,int ip){
   for(int i=0;i<pl_list->count;i++){
      if(pl_list->players[i].ip==ip){
         for(int j=i;j+1<pl_list->count;j++){
            pl_list->players[j]=pl_list->players[j+1];
         }
         pl_list->count-=1;
         return;
      }
   }
}

void print_data(Player_list *pl_list){
   printf("----\n");
   for(int i=0;i<pl_list->count;i++){
      printf("%d x=%d y=%d\n",pl_list->players[i].ip,pl_list->players[i].cord.x,pl_list->players[i].cord.y);
   }
   printf("----\n");
}


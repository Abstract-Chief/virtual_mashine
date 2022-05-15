#include<unistd.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"module.h"
#include"structure.h"

void error(char *str){
   perror(str);
   exit(1);
}

int create_server(){
   int my_socket,opt=1;
   my_socket=socket(AF_INET,SOCK_STREAM,0);
   setsockopt(my_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
   if(my_socket<0){
      error("mysocket");
   }
   struct sockaddr_in addr;
   addr.sin_family=AF_INET;
   addr.sin_port=htons(12345);
   addr.sin_addr.s_addr=htons(INADDR_ANY);
   if(bind(my_socket,(struct sockaddr*)&addr,sizeof(addr))<0){
      error("bind");
   }
   if(listen(my_socket,10)==-1){
      error("listen");
   }
   return my_socket;
}

void klient_fd_create(Fd_talbe *table){
   table->count=0;
   table->data=calloc(100,sizeof(unsigned char));
}

void clean_set(Fd_talbe *table){
   FD_ZERO(&table->read);
   FD_ZERO(&table->write);
}
void update_socket(unsigned char my_socket,Fd_talbe *table){
   table->maxfd=my_socket;
   FD_SET(my_socket,&table->read);
   for(int i=0;table->data[i];i++){
      FD_SET(table->data[i],&table->read);
      if(table->maxfd<table->data[i]){
         table->maxfd=table->data[i];
      }
   }
}
void remove_el(unsigned char *arr,int i){
   arr[i++]=0;
   for(;arr[i];i++){
      arr[i-1]=arr[i];
   }
}

char *get_from_packet(char packet){
   char *otv=malloc(2);
   char packet_copy=packet;
   otv[0]=packet>>4;
   otv[1]=(packet_copy<<4);
   otv[1]=otv[1]>>4;
   return otv;
}

void server_handler(unsigned char sock,char type,char packet,Player_list *pl_list,int index){
   switch(type){
      case(1):;
         char *cord=get_from_packet(packet);
         move_player(&pl_list->players[index],cord[1],cord[0]);
         break;
      case(2):;
         char type_send=3;
         Player_list pl_list_copy;
         memcpy(&pl_list_copy,pl_list,sizeof(pl_list));
         pl_list_copy.count=pl_list->count;
         delete_user(&pl_list_copy,pl_list_copy.players[index].ip);
         write(sock,&type_send,1);
         write(sock,&pl_list_copy,sizeof(*pl_list));
         /*write(sock,pl,1);*/
         /*write(sock,packet_send,2+20*sizeof(Player));*/
         break;
   }
}

char read_packet(unsigned char sock,char *packet){
   int type;
   if(read(sock,&type,1)<=0)
      return 0;   
   if(type==2){
      packet=NULL;
      return type;
   }
   if(read(sock,packet,1)<=0)
      return 0;
   return type;
}

size_t size_addr=sizeof(struct sockaddr);
int select_handler(int select_result,unsigned char my_socket,Fd_talbe *table,Player_list *pl_list){
   if(select_result<1)
      error("select");
   if(FD_ISSET(my_socket,&table->read)){//принимаем запрос на подключение
      struct sockaddr addr;
      table->data[table->count++]=accept(my_socket,&addr,&size_addr);
      add_user(&pl_list->players[pl_list->count++],(struct sockaddr_in *)&addr);
      printf("add player\n");
      return 0;
   }
   for(int i=0;table->data[i];i++){
      if(FD_ISSET(table->data[i],&table->read)){
         char packet;
         char type=read_packet(table->data[i],&packet);
         if(type==0){
            printf("kill socket - %d\n",table->data[i]);
            close(table->data[i]);
            remove_el(table->data,i);
            table->count--;
         }
         server_handler(table->data[i],type,packet,pl_list,i);
      }
      print_data(pl_list);
   }
   return 0;
}

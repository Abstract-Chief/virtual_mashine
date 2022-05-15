
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"structure.h"
#include"internet.h"


int main(int argc,char *argv[]){
   int sock=create_server();
   Fd_talbe fd_table;
   klient_fd_create(&fd_table);
   Player_list list;
   list.count=0;
   while(1){
      update_socket(sock,&fd_table);
      int res=select(fd_table.maxfd+1,&fd_table.read,NULL,NULL,NULL);
      select_handler(res,sock,&fd_table,&list);
   }
}

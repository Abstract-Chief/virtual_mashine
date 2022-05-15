#include<stdlib.h>
#include<stdio.h>
#include <netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<ncurses.h>
#include<time.h>

#include"internet.h"
#include"../server/structure.h"

#define PORT 12345
#define CONECT_TEXT "conecting"
#define ERROR_CONECT_TEXT "error connect"
#define BUTTON_RECCONECT_TEXT "press enter for restart"
enum keys{
   up_key=3,
   down_key=2,
   left_key=4,
   right_key=5,
};

int MAX_Y,MAX_X;

void check_args(int argc){
   if(argc<2){
      printf("to few args\nplease enter server addres\n");
      exit(1);
   }
}
int get_center_x_cord(int max_x,char *text){
   return (max_x/2)-(strlen(text)/2);
}

int conect_server(char *ip,struct sockaddr_in* addr){
   int sock;
   do{
      clear();
      mvprintw(MAX_Y/2-1,get_center_x_cord(MAX_X,CONECT_TEXT),CONECT_TEXT);
      refresh();
      sock=Conect(ip,12345,addr);
      sleep(1);
      if(sock<0){
         attron(COLOR_PAIR(3));
         mvprintw(MAX_Y/2-1,get_center_x_cord(MAX_X,ERROR_CONECT_TEXT),ERROR_CONECT_TEXT);
         attroff(COLOR_PAIR(3));
         mvprintw(MAX_Y/2,get_center_x_cord(MAX_X,BUTTON_RECCONECT_TEXT),BUTTON_RECCONECT_TEXT);
         refresh();
         getchar();
      }
   }while(sock<0);
   clear();
   refresh();
   return sock;
}

unsigned char create_packet(char a,char b){
   if(a>15 || b>15)
      return -1;
   return ((a << 4) >> 4)|((b << 4));
}
void print_data(Player_list *pl_list){
   printf("----\n");
   for(int i=0;i<pl_list->count;i++){
      printf("%d x=%d y=%d\n",pl_list->players[i].ip,pl_list->players[i].cord.x,pl_list->players[i].cord.y);
   }
   printf("----\n");
}

int move_user(int sock,int route){
   int data=create_packet(route,1);
   Send(sock,1,&data,1);
   return 0;
}
int main(int argc,char *args[]){
   initscr();
      getmaxyx(stdscr,MAX_Y,MAX_X);
      curs_set(0);
      cbreak();
      noecho();
      keypad(stdscr,1);
      /*nodelay(stdscr,1);*/
   start_color();
      init_pair(3,COLOR_RED,COLOR_BLACK);   
      init_pair(1,COLOR_RED,COLOR_RED);   

   struct sockaddr_in my_addr;
   int sock=conect_server(args[1],&my_addr);
   /*int data=create_packet(2,4);*/
   /*Send(sock,2,&data,1);*/
   /*char type;*/
   /*Player_list pl_list;*/
   /*read(sock,&type,1);*/
   /*read(sock,&pl_list,sizeof(pl_list));*/
   /*print_data(&pl_list);*/
   /*while(1);*/
   unsigned char input;
   char flag=0;
   int x=5,y=5;
   struct timespec sc={0,100000000};
   while(1){
      input=getch();
      clear();
      switch(input){
         case(up_key): y--; move_user(sock,3); break;
         case(down_key): y++; move_user(sock,1); break;
         case(right_key): x++; move_user(sock,0); break;
         case(left_key): x--; move_user(sock,2); break;
      }
      mvprintw(y,x,"#");  
      refresh();
      nanosleep(&sc,NULL);
   }
}

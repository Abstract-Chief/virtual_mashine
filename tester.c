#include<arpa/inet.h>

#include<unistd.h>
#include<stdlib.h>

/*не интернет фунции*/
void error(const char *msg){
   exit(1);
}


struct sockaddr_in GetServAddr(char *ip,int port){
   struct sockaddr_in server_addr;
   server_addr.sin_family=AF_INET;
   server_addr.sin_port=htons(12345);
   server_addr.sin_addr.s_addr=inet_addr(ip);
   if(server_addr.sin_addr.s_addr<0)
      error("addr");
   return server_addr;
}

int CreateSocket(struct sockaddr_in *addr){
   int my_socket,opt=1;
   my_socket=socket(AF_INET,SOCK_STREAM,0);
   setsockopt(my_socket,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
   if(my_socket<0){
      error("mysocket");
   }
   addr->sin_family=AF_INET;
   addr->sin_port=htons(1235);
   addr->sin_addr.s_addr=htons(INADDR_ANY);
   if(bind(my_socket,(struct sockaddr*)addr,sizeof(*addr))<0){
      error("bind");
   }
   return my_socket;
}

int Conect(char *ip,int port,struct sockaddr_in *addr){
   int my_socket=CreateSocket(addr);
   struct sockaddr_in server_addr=GetServAddr(ip,port);
   socklen_t size=(socklen_t)sizeof(server_addr);
   if(connect(my_socket,(struct sockaddr *)&server_addr,size)<0)
      return -1;
   return my_socket;
}

void Send(int sock,char signal,void *data,size_t size){
   if(write(sock,&signal,sizeof(signal))<0)  
      exit(1);
   if(size==0)
      return ;
   if(write(sock,data,size)<0)
      exit(1);
}

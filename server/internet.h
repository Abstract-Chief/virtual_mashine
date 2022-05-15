#ifndef INTERNET_H_SENTURY
#define INTERNET_H_SENTURY
int create_server();
void klient_fd_create(Fd_talbe *table);
void server_handler(unsigned char sock,char *packet);
int select_handler(int select_result,unsigned char my_socket,Fd_talbe *table,Player_list *pl_list);
void update_socket(unsigned char my_socket,Fd_talbe *table);
#endif

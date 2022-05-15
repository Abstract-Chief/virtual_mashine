#ifndef MODULE_H_SENTURY
#define MODULE_H_SENTURY
#include"structure.h"
void *send_user_cord_packet(Player_list *pl_list,int ip);
void print_data(Player_list *pl_list);
int move_player(Player *user,int route,int step);
void add_user(Player *user,struct sockaddr_in *addr);
void delete_user(Player_list *pl_list,int ip);
#endif

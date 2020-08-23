#ifndef _TCPNET_H
#define _TCPNET_H
#include <TCPKernel.h>


struct sockaddr_in serveraddr;

struct epoll_event epollarr[_DEF_EPOLLSIZE];
int sockfd;
int clientfd;
int epfd;



int InitNetWork();
void UnInitNetWork();
void * EPOLL_Jobs(void*);
void *Accept_Deal(void*);
void *Info_Recv(void*);
int SendData(int,char*,int);
void Addfd(int,int);
void Deletefd(int);
void Epoll_Deal(int,pool_t*);



#endif

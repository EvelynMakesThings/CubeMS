#ifndef NET_H
#define NET_H

// Initilize network variables and socket.
void netInit(int* sockfd);
void netReceive(int* sockfd,char* BUFFER,struct sockaddr_in* client_addr);

#endif
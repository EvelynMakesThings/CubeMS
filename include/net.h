#ifndef NET_H
#define NET_H

// Initilize network variables and socket.
void netInit(int* sockfd);
// Receive a network request, returns clientfd
int netReceive(int* sockfd,char* BUFFER,struct sockaddr_in* client_addr);
// Respond to a network request (Modify this function the most)
void netRespond(int* connfd,char* BUFFER,struct sockaddr_in* client_addr);

#endif
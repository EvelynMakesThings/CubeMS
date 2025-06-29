#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <CubeMS.h>
#include <arpa/inet.h>

void netInit(int* sockfd) {
    // Create the socket file descriptor
    *sockfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (*sockfd < 0) {
        perror("Failed to create network socket.\n");
        exit(EXIT_FAILURE);
    }

    // Create the server address value
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind to socket
    int bind_success = bind(*sockfd,(const struct sockaddr *)&server_addr,sizeof(struct sockaddr_in));
    if (bind_success < 0) {
        perror("Failed to bind to socket");
        exit(EXIT_FAILURE);
    }

    int listen_success = listen(*sockfd,5);
    if (listen_success != 0) {
        perror("Failed to start listening");
        exit(EXIT_FAILURE);
    }
    return;
}

void netReceive(int* sockfd,char* BUFFER,struct sockaddr_in* client_addr) {
    // Clear buffer
    bzero(BUFFER,BUFF_SIZE);
    // Accept incoming request
    int len = sizeof(struct sockaddr_in);
    int connfd = accept(*sockfd, (struct sockaddr*)client_addr, &len); 
    if (connfd < 0) { 
        perror("Accept faliure"); 
        exit(EXIT_FAILURE); 
    }
    
    read(connfd,BUFFER,BUFF_SIZE);
    BUFFER[BUFF_SIZE] = '\0';
    printf("Request from %s:%d\n",inet_ntoa(client_addr->sin_addr),(int)client_addr->sin_port);
    #ifdef DEBUG
        puts(BUFFER);
    #endif
    return;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CubeMS.h>
#include <arpa/inet.h>

void initNet(int* sockfd, struct sockaddr_in* server_addr) {
    // Create the socket file descriptor
    *sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (*sockfd < 0) {
        perror("Failed to create network socket.\n");
        exit(EXIT_FAILURE);
    }

    // Create the server address value
    bzero(server_addr,sizeof(struct sockaddr_in));
    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(PORT);

    // Bind to socket
    int bind_success = bind(*sockfd,(const struct sockaddr *)server_addr,sizeof(struct sockaddr_in));
    if (bind_success < 0) {
        perror("Failed to bind to socket");
        exit(EXIT_FAILURE);
    }
}
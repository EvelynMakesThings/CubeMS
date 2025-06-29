#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <CubeMS.h>
#include <arpa/inet.h>

void netMakeResponse(char* BUFFER,char* body) {
    snprintf(BUFFER, BUFF_SIZE,
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(body), body);
}

void netInit(int* sockfd) {
    // Create the socket file descriptor
    *sockfd = socket(AF_INET,SOCK_STREAM,0);
    int opt = 1;
    setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Reuse because TCP sucks
    if (*sockfd < 0) {
        perror("Failed to create network socket.\n");
        exit(EXIT_FAILURE);
    }

    // Create the server address value
    struct sockaddr_in server_addr;
    memset(&server_addr,0x0,sizeof(struct sockaddr_in));
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

int netReceive(int* sockfd,char* BUFFER,struct sockaddr_in* client_addr) {
    // Clear buffer
    memset(BUFFER,0x0,BUFF_SIZE);
    // Accept incoming request
    int len = sizeof(struct sockaddr_in);
    int connfd = accept(*sockfd, (struct sockaddr*)client_addr, &len); 
    if (!running) {
        return 0;
    }
    if (connfd < 0) {
        perror("Accept faliure"); 
        exit(EXIT_FAILURE); 
    }
    
    read(connfd,BUFFER,BUFF_SIZE);
    BUFFER[BUFF_SIZE] = '\0';
    return connfd;
}

void netRespond(int* connfd,char* BUFFER,struct sockaddr_in* client_addr) {
    // Check if GET request in HTTP
    int GET = strncmp(BUFFER,"GET ",4);
    if (GET != 0) {
        puts("Not a get request... Ignoring.");
        close(*connfd);
        return;
    }
    // Get path
    char *pathStart = BUFFER + 4;
    char *pathEnd = strstr(pathStart, " ");
    if (pathEnd == NULL) { puts("Bad path... Ignoring."); close(*connfd); return; } // If NULL, invalid GET request. Ignore.
    size_t pathLen = pathEnd - pathStart;
    char path[1024];
    strncpy(path, pathStart, pathLen);
    path[pathLen] = 0;
    if (strncmp(path,basePath,strlen(basePath)) != 0) {
        puts("Base path does not match..."); // Update to respond with 404 later.
        close(*connfd);
        return;
    }
    char* subPath = path+strlen(basePath);

    #ifdef DEBUG
        printf("Detected path: %s\n",path);
        puts(subPath);
    #endif

    memset(BUFFER,0x0,BUFF_SIZE);

    if (strncmp(subPath,"register.do?action=add",strlen("register.do?action=add")) == 0) {
        puts("register");
        netMakeResponse(BUFFER,"Registered.");
        send(*connfd,BUFFER,strlen(BUFFER),0);
        close(*connfd);
    } else {
        puts("Path not found..."); // Update to respond with 404 later.
    }
}
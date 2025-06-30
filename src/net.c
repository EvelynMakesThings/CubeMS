#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <arpa/inet.h>
#include "CubeMS.h"
#include "net.h"
#include "tools.h"

void netMakeResponse(char* BUFFER,char* body,char* status) {
    snprintf(BUFFER, BUFF_SIZE,
        "HTTP/1.0 %s\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        status, strlen(body), body);
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
    BUFFER[BUFF_SIZE-1] = '\0';
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
        netMakeResponse(BUFFER,"Path-not-found.\n","404 Not Found");
        printf("Path \"%s\" not found.\n",path);
        send(*connfd,BUFFER,strlen(BUFFER),0);
        close(*connfd);
        return;
    }
    char* subPath = path+strlen(basePath);

    #ifdef DEBUG
        printf("Detected path: %s\n",path);
        puts(subPath);
    #endif

    // Try to extract X-Forwarded-For if behind proxy
    char real_ip[INET_ADDRSTRLEN] = {0};
    char* xff = strstr(BUFFER, "X-Forwarded-For: ");
    if (xff) {
        xff += strlen("X-Forwarded-For: ");
        char* xff_end = strchr(xff, '\r');
        if (!xff_end) xff_end = strchr(xff, '\n');
        if (xff_end && (size_t)(xff_end - xff) < INET_ADDRSTRLEN) {
            strncpy(real_ip, xff, xff_end - xff);
            real_ip[xff_end - xff] = '\0';
        }
    }
    if (real_ip[0] == '\0') {
        strncpy(real_ip, inet_ntoa(client_addr->sin_addr), INET_ADDRSTRLEN);
    }

    memset(BUFFER,0x0,BUFF_SIZE);
    char body[BUFF_SIZE] = {0x0};

    if (strncmp(subPath,"register.do?action=add",strlen("register.do?action=add")) == 0) {
        puts("Register request.");
        int succ = UpdateServer(Addresses,real_ip);
        if (succ < 0) {
            puts("Could not find a slot! Server count full...");
            netMakeResponse(BUFFER,"Failed to register... Too many servers.\n","500 Internal Server Error");
        } else {
            #ifdef DEBUG
                if (succ == 0) {
                    puts("New slot used.");
                } else {
                    puts("Reused old slot.");
                }
            #endif
            netMakeResponse(BUFFER,"Registered.\n","200 OK");
        }
    } else if (strncmp(subPath,"retrieve.do?item=list",strlen("retrieve.do?item=list")) == 0) {
        puts("List request.");
        for (int i = 0; i < MAX_SERVERS; i++) {
            #ifdef DEBUG
                printf("Slot %d: active=%d, address='%s'\n", i, Addresses[i].active, Addresses[i].address);
            #endif
            strcpy(body,"echo The server is under development... Expect issues.\n");
            if (Addresses[i].active == 0x1) {
                AddServer(body,Addresses[i].address);
            }
            netMakeResponse(BUFFER,body,"200 OK");
        }
    } else {
        printf("Path \"%s\" not found.\n",path);
        netMakeResponse(BUFFER,"Path-not-found.\n","404 Not Found");
    }
    send(*connfd,BUFFER,strlen(BUFFER),0);
    close(*connfd);
}
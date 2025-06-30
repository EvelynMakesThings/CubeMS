#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 
#include <string.h>
#include <arpa/inet.h>
#include "CubeMS.h"
#include "tools.h"
#include "net.h"
#include <tools.h>
#include <time.h>

int sockfd,connfd;
struct sockaddr_in client_addr;
char BUFFER[BUFF_SIZE];
ip Addresses[MAX_SERVERS];
int running = 1;
const char* basePath = "/"; // Should ALWAYS end with a "/".

void sigintHandler(int sig_num) {
    puts("\nStop signal recieved.");
    close(sockfd);
    running = 0;
    return;
}

int main() {

    #ifdef DEBUG
        puts("DEBUG BUILD");
    #endif

    signal(SIGINT, sigintHandler); // Bind ^C to our custom termination
    puts("NET_INIT");
    netInit(&sockfd);
    memset(Addresses, 0x0, sizeof(Addresses));
    puts("NET_LOOP");
    while (1) {
        connfd = netReceive(&sockfd,BUFFER,&client_addr); // Wait for a request
        if (!running) {break;} // Quit loop if ^C pressed.
        printf("Request from %s:%d\n",inet_ntoa(client_addr.sin_addr),(int)client_addr.sin_port);
        TickServers(Addresses);
        #ifdef DEBUG
            puts(BUFFER); // Compile with -DDEBUG or "make debug" for DEBUG to work in the ifdef
        #endif
        netRespond(&connfd,BUFFER,&client_addr);
        #ifdef DEBUG
            puts(BUFFER); // Compile with -DDEBUG or "make debug" for DEBUG to work in the ifdef
        #endif
    }
    puts("Stopping...");
    return EXIT_SUCCESS;
}
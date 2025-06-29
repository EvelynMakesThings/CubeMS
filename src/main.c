#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 
#include <string.h>
#include <arpa/inet.h>
#include <CubeMS.h>
#include <net.h>
#include <tools.h>

int sockfd;
struct sockaddr_in client_addr;
char BUFFER[BUFF_SIZE];
int running = 1;

void sigintHandler(int sig_num) {
    puts(" recieved... Waiting for next reply.");
    running = 0;
}

int main() {
    signal(SIGINT, sigintHandler);
    puts("NET_INIT");
    netInit(&sockfd);
    //printf("Server listening on localhost:%d.\n",PORT);
    puts("NET_LOOP");
    while (running) {
        netReceive(&sockfd,BUFFER,&client_addr);
    }
    puts("Stopping...");
    close(sockfd);
    return 0;
}

/* 
int main() {
    ServerInfo *test = malloc(sizeof(ServerInfo)); // Placeholder Data
    strcpyS(test->ip,"example.com",sizeof(test->ip));
    strcpyS(test->desc,"Example Description",sizeof(test->desc));
    strcpyS(test->map,"default",sizeof(test->map));
    test->players = 1;
    test->maxplayers = 20;
    printf("        Server Printout\n");
    printf("--------------------------------\n");
    printf("Server Address:  %s\n",test->ip);
    printf("Server Desc:     %s\n",test->desc);
    printf("Current Players: %d\n",test->players);
    printf("Max Players:     %d\n",test->maxplayers);
    printf("Current Map:     %s\n",test->map);
    return 0;
}
*/
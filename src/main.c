#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h> 
#include <string.h>
#include <arpa/inet.h>
#include "CubeMS.h"
#include "tools.h"
#include "net.h"
#include <time.h>
#include <err.h>
#include <errno.h>

int sockfd,connfd;
struct sockaddr_in client_addr;
char BUFFER[BUFF_SIZE];
ip Addresses[MAX_SERVERS];
int running = 1;
char basePath[BUFF_SIZE] = "/";
int PORT = d_PORT;
int TTL = d_TTL;

void sigintHandler(int sig_num) {
    puts("\nStop signal recieved.");
    close(sockfd);
    running = 0;
    return;
}

int main(int argc, char *argv[]) {

    #ifdef DEBUG
        puts("DEBUG BUILD");
    #endif

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0) {
            printf(
                "Cube Master Server\n\n"
                "    -h, --help: Displays this text.\n\n"
                "    -p, --port: Changes the port the\n"
                "                server runs on.\n"
                "                (Ex: CubeMS -p 8080)\n\n"
                "    -b, --base: Sets the base path\n"
                "                that the server uses.\n"
                "                Must begin and end with\n"
                "                a \"/\" character.\n"
                "                (Ex: CubeMS -b /cube/masterserver/)\n");
            return 0;
        } else if (strcmp(argv[i],"-p") == 0 || strcmp(argv[i],"--port") == 0) {
            if ( i + 1 >= argc) {
                fputs("No argument for port.\n",stderr);
                return 1;
            }
            char *endptr;
            errno = 0;
            PORT = strtol(argv[i+1],&endptr,10);
            if ( errno == EINVAL || errno == ERANGE ) {
                perror("Argument error");
                return 1;
            }
            if ( PORT > 65535 || PORT < 1 || *endptr != '\0') {
                fputs("Invalid port number.\n",stderr);
                return 1;
            }
            i++;
        } else if (strcmp(argv[i],"-b") == 0 || strcmp(argv[i],"--base") == 0) {
            if ( i + 1 >= argc) {
                fputs("No argument for base path.\n",stderr);
                return 1;
            }
            // VERY MESSY APPROACH TO INPUT VALIDATION- PLEASE OH PLEASE FORGIVE ME FOR MY SINS.
            int invalidBasePath = 0;
            if ( strstr(argv[i+1],"!") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"*") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"'") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"(") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],")") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],";") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],":") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"@") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"&") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"=") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"+") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"$") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],",") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"?") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"#") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"[") != NULL ) {invalidBasePath = 1;}
            if ( strstr(argv[i+1],"]") != NULL ) {invalidBasePath = 1;}
            char* s = argv[i + 1];
            for (int j = 0; s[j]!='\0'; j++) {
                int c = (int)s[j];
                if (c < 32 || c > 127) { // Make sure in valid ascii range (if not that is bad)
                    invalidBasePath = 1;
                }
            }
            if (invalidBasePath==1) {
                fputs("Invalid characters in path.\n",stderr);
                return 1;
            }
            if (s[0] != '/' || s[strlen(s) - 1] != '/') {
                fputs("Base path must begin and end with '/'.\n", stderr);
                return 1;
            }
            if (strlen(s) > BUFF_SIZE) {
                fputs("Path is too long.\n", stderr);
                return 1;
            }
            strcpy(basePath,s);
            i++;
        } else {
            fprintf(stderr,"Unknown option \"%s\".\n",argv[i]);
            return 1;
        }
    }

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
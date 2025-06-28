#include <arpa/inet.h>

#ifndef CUBEMS_H
#define CUBEMS_H

// Port to listen on
#define PORT 80
#define BUFF_SIZE 1024

// Server Info Structure (Recieved from servers and temporarily stored.)
typedef struct ServerInfo {
	char ip[INET_ADDRSTRLEN];
	char desc[128];
	int players;
	int maxplayers;
	char map[64];
} ServerInfo;

#endif

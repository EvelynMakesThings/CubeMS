#include <netinet/in.h>

#ifndef CUBEMS_H
#define CUBEMS_H

// Server Info Structure (Recieved from servers and temporarily stored.)
typedef struct ServerInfo {
	char ip[INET_ADDRSTRLEN];
	char desc[128];
	int players;
	int maxplayers;
	char map[64];
} ServerInfo;

#endif

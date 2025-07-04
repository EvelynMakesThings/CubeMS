#include <arpa/inet.h>

#ifndef CUBEMS_H
#define CUBEMS_H

// These are the DEFAULT values. These will be modifiable via arguments.
#define d_PORT 80
#define d_TTL 3600 // 1 hour, the default value in dedicated cube servers.

#define BUFF_SIZE 2048
#define MAX_SERVERS 100

typedef struct ip {
	char	active;
	char	address[INET_ADDRSTRLEN];
	time_t	time;
} ip;

extern int running;
extern char basePath[BUFF_SIZE];
extern int PORT;
extern int TTL;
extern ip Addresses[MAX_SERVERS];

#endif

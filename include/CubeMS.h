#include <arpa/inet.h>

#ifndef CUBEMS_H
#define CUBEMS_H

#define PORT 80
#define BUFF_SIZE 2048
#define MAX_SERVERS 100
#define TTL 3600 // 1 hour, the default value in dedicated cube servers.

typedef struct ip {
	char	active;
	char	address[INET_ADDRSTRLEN];
	time_t	time;
} ip;

extern int running;
extern const char* basePath;
extern ip Addresses[MAX_SERVERS];

#endif

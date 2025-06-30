#include <stddef.h>
#include "CubeMS.h"
#ifndef TOOLS_H
#define TOOLS_H

// Safer string copy, ensure null terminated string.
void strcpyS(char *__restrict__ __dest, const char *__restrict__ __src, size_t __n);
// Append to null terminated string of BUFF_SIZE length.
void strcpyA(char* __dest, const char* __src);
// Add a server to buffer, should be safe
void AddServer(char* BUFF, char* Server);
// Allocate / Renew a server in the server slots.
int UpdateServer(ip Addresses[],const char* address);
// Return remaining servers after disabling timed out servers.
int TickServers(ip Addresses[]);

#endif
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "CubeMS.h"
void strcpyS(char *__restrict__ __dest, const char *__restrict__ __src, size_t __n) {
    strncpy(__dest,__src,__n); // Regular strncpy (I love memory safe code)
    __dest[__n-1] = '\0'; // Ensure null terminated
}

void strcpyA(char* __dest, const char* __src) {
    size_t len = strlen(__dest);
    strcpyS(__dest+len,__src,BUFF_SIZE-len);
}

void AddServer(char* BUFF, char* Server) {
    strcpyA(BUFF,"addserver ");
    strcpyA(BUFF,Server);
    strcpyA(BUFF,"\n");
}

int UpdateServer(ip Addresses[],const char* address) {
    for (int i = 0; i < MAX_SERVERS; i++) {
        if (strncmp(Addresses[i].address,address,INET_ADDRSTRLEN) == 0 && Addresses[i].active) {
            Addresses[i].active = 0x1;
            Addresses[i].time = time(NULL);
            return 1;
        }
    }
    for (int i = 0; i < MAX_SERVERS; i++) {
        if (Addresses[i].active == 0x0) {
            strcpyS(Addresses[i].address,address,INET_ADDRSTRLEN);
            Addresses[i].active = 0x1;
            Addresses[i].time = time(NULL);
            return 0;
        }
    }
    return -1;
}

int TickServers(ip Addresses[]) {
    int count = 0;
    for (int i = 0; i < MAX_SERVERS; i++) {
        if (Addresses[i].active == 0x1) {
            if ((Addresses[i].time+TTL)<time(NULL)) {
                puts("Deactivating server...");
                Addresses[i].active = 0x0;
            } else {
                count++;
            }
        }
    }
    return count;
}
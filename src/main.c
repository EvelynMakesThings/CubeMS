#include <stdio.h>
#include "CubeMS.h"

int main() {
    ServerInfo test = {"example.com","Example Server Structure",1,20,"default"}; // Placeholder Data
    printf("        Server Printout\n");
    printf("--------------------------------\n");
    printf("Server Address:  %s\n",test.ip);
    printf("Server Desc:     %s\n",test.desc);
    printf("Current Players: %d\n",test.players);
    printf("Max Players:     %d\n",test.maxplayers);
    printf("Current Map:     %s\n",test.map);
    return 0;
}

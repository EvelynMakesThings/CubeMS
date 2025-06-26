#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CubeMS.h>
#include <tools.h>

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

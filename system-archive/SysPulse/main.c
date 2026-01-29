#include "server.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc > 1) {
        port = atoi(argv[1]);
        if (port <= 0) port = 8080;
    }
    printf("SysPulse starting on port %d\n", port);
    start_server(port);
    return 0;
}


#include "router.h"
#include "handler.h"
#include <string.h>
#include <stdio.h>

void route_request(int client_sock, const char *path) {
    // Static file endpoints
    if (strcmp(path, "/") == 0) {
        serve_index(client_sock);
    } else if (strcmp(path, "/style.css") == 0) {
        serve_static(client_sock, "html/style.css", "text/css");
    } else if (strcmp(path, "/script.js") == 0) {
        serve_static(client_sock, "html/script.js", "application/javascript");
    }
    // API endpoints
    else if (strcmp(path, "/api/uptime") == 0) {
        serve_uptime_json(client_sock);
    } else if (strcmp(path, "/api/load") == 0) {
        serve_load_json(client_sock);
    } else if (strcmp(path, "/api/mem") == 0) {
        serve_mem_json(client_sock);
    } else if (strcmp(path, "/api/cpu") == 0) {
        serve_cpu_json(client_sock);
    } else if (strcmp(path, "/api/disk") == 0) {
        serve_disk_json(client_sock);
    } else {
        serve_not_found(client_sock);
    }
}


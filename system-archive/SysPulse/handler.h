#ifndef HANDLER_H
#define HANDLER_H

void serve_index(int client_sock);
void serve_static(int client_sock, const char *filepath, const char *mime_type);
void serve_not_found(int client_sock);

void serve_uptime_json(int client_sock);
void serve_load_json(int client_sock);
void serve_mem_json(int client_sock);
void serve_cpu_json(int client_sock);
void serve_disk_json(int client_sock);

#endif


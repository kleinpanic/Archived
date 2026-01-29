#include "handler.h"
#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 8192

// Helper function to send an HTTP response
static void send_response(int client_sock, const char *status, const char *content_type, const char *body) {
    char response[BUFFER_SIZE];
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n%s",
             status, content_type, strlen(body), body);
    write(client_sock, response, strlen(response));
}

void serve_index(int client_sock) {
    FILE *file = fopen("html/index.html", "r");
    if (!file) {
        serve_not_found(client_sock);
        return;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    char *content = malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);
    send_response(client_sock, "200 OK", "text/html", content);
    free(content);
}

void serve_static(int client_sock, const char *filepath, const char *mime_type) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        serve_not_found(client_sock);
        return;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    char *content = malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';
    fclose(file);
    send_response(client_sock, "200 OK", mime_type, content);
    free(content);
}

void serve_not_found(int client_sock) {
    const char *body = "<h1>404 Not Found</h1>";
    send_response(client_sock, "404 Not Found", "text/html", body);
}

// API endpoint: /api/uptime
void serve_uptime_json(int client_sock) {
    char json[256];
    double uptime;
    if (get_uptime(&uptime) != 0) {
        snprintf(json, sizeof(json), "{\"error\": \"Failed to get uptime\"}");
    } else {
        snprintf(json, sizeof(json), "{\"uptime\": %.2f}", uptime);
    }
    send_response(client_sock, "200 OK", "application/json", json);
}

// API endpoint: /api/load
void serve_load_json(int client_sock) {
    char json[256];
    double load1, load5, load15;
    if (get_load(&load1, &load5, &load15) != 0) {
        snprintf(json, sizeof(json), "{\"error\": \"Failed to get load average\"}");
    } else {
        snprintf(json, sizeof(json), "{\"load1\": %.2f, \"load5\": %.2f, \"load15\": %.2f}", load1, load5, load15);
    }
    send_response(client_sock, "200 OK", "application/json", json);
}

// API endpoint: /api/mem
void serve_mem_json(int client_sock) {
    char json[256];
    long mem_total, mem_free;
    if (get_mem(&mem_total, &mem_free) != 0) {
        snprintf(json, sizeof(json), "{\"error\": \"Failed to get memory info\"}");
    } else {
        snprintf(json, sizeof(json), "{\"mem_total\": %ld, \"mem_free\": %ld}", mem_total, mem_free);
    }
    send_response(client_sock, "200 OK", "application/json", json);
}

// API endpoint: /api/cpu
void serve_cpu_json(int client_sock) {
    char json[512];
    char cpu_stats[256];
    if (get_cpu(cpu_stats, sizeof(cpu_stats)) != 0) {
        snprintf(json, sizeof(json), "{\"error\": \"Failed to get CPU info\"}");
    } else {
        snprintf(json, sizeof(json), "{\"cpu_stats\": \"%s\"}", cpu_stats);
    }
    send_response(client_sock, "200 OK", "application/json", json);
}

// API endpoint: /api/disk
void serve_disk_json(int client_sock) {
    char json[256];
    long total, free;
    if (get_disk(&total, &free) != 0) {
        snprintf(json, sizeof(json), "{\"error\": \"Failed to get disk info\"}");
    } else {
        snprintf(json, sizeof(json), "{\"disk_total\": %ld, \"disk_free\": %ld}", total, free);
    }
    send_response(client_sock, "200 OK", "application/json", json);
}


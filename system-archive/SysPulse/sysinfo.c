#include "sysinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include <unistd.h>

int get_uptime(double *uptime) {
    FILE *file = fopen("/proc/uptime", "r");
    if (!file) return -1;
    if (fscanf(file, "%lf", uptime) != 1) {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

int get_load(double *load1, double *load5, double *load15) {
    FILE *file = fopen("/proc/loadavg", "r");
    if (!file) return -1;
    if (fscanf(file, "%lf %lf %lf", load1, load5, load15) != 3) {
        fclose(file);
        return -1;
    }
    fclose(file);
    return 0;
}

int get_mem(long *mem_total, long *mem_free) {
    FILE *file = fopen("/proc/meminfo", "r");
    if (!file) return -1;
    char line[256];
    *mem_total = 0;
    *mem_free = 0;
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "MemTotal: %ld kB", mem_total) == 1)
            continue;
        if (sscanf(line, "MemAvailable: %ld kB", mem_free) == 1)
            break;
    }
    fclose(file);
    if (*mem_total == 0 || *mem_free == 0) return -1;
    return 0;
}

int get_cpu(char *buffer, size_t buflen) {
    FILE *file = fopen("/proc/stat", "r");
    if (!file) return -1;
    if (!fgets(buffer, buflen, file)) {
        fclose(file);
        return -1;
    }
    fclose(file);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n')
        buffer[len - 1] = '\0';
    return 0;
}

int get_disk(long *total, long *free) {
    struct statvfs stat;
    if (statvfs("/", &stat) != 0) return -1;
    *total = (stat.f_blocks * stat.f_frsize) / 1024;
    *free = (stat.f_bfree * stat.f_frsize) / 1024;
    return 0;
}


#ifndef SYSINFO_H
#define SYSINFO_H

#include <stddef.h>

int get_uptime(double *uptime);
int get_load(double *load1, double *load5, double *load15);
int get_mem(long *mem_total, long *mem_free);
int get_cpu(char *buffer, size_t buflen);
int get_disk(long *total, long *free);

#endif


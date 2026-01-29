#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Add this for sysconf()
#include "cpu_control.h"

void set_cpu_governor(const char *governor) {
    int cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
    for (int i = 0; i < cpu_count; i++) {
        char path[64];
        sprintf(path, "/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor", i);
        FILE *f = fopen(path, "w");
        if (f) {
            fprintf(f, "%s", governor);
            fclose(f);
        } else {
            perror("Failed to set CPU governor");
        }
    }
}

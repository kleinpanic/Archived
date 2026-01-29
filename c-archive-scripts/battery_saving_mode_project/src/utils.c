#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void log_status(const char *message) {
    FILE *logfile = fopen("/var/log/battery_saver.log", "a");
    if (logfile) {
        fprintf(logfile, "%s\n", message);
        fclose(logfile);
    } else {
        perror("Failed to write to log");
    }
}

void print_status() {
    printf("CPU Governor: ");
    system("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor");
    printf("Brightness: ");
    system("cat /sys/class/backlight/intel_backlight/brightness");
    printf("WiFi Status: ");
    system("nmcli radio wifi");
}



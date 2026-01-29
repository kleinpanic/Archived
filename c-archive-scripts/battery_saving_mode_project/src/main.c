#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "cpu_control.h"
#include "brightness_control.h"
#include "service_control.h"
#include "utils.h"

void print_usage() {
    printf("Usage: battery_saver --enable | --disable | --status\n");
    printf("--enable   : Enable battery saving mode\n");
    printf("--disable  : Disable battery saving mode and restore settings\n");
    printf("--status   : Display current battery saving mode status\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int enable = 0, disable = 0, status = 0;

    struct option long_options[] = {
        {"enable", no_argument, &enable, 1},
        {"disable", no_argument, &disable, 1},
        {"status", no_argument, &status, 1},
        {0, 0, 0, 0}
    };

    if (argc < 2) {
        print_usage();
        return 1;
    }

    while ((opt = getopt_long(argc, argv, "", long_options, NULL)) != -1) {
        if (opt == '?') {
            print_usage();
            return 1;
        }
    }

    if (enable) {
        set_cpu_governor("powersave");
        adjust_brightness(20);  // Set to 20% brightness
        disable_services();
        log_status("Battery saving mode enabled.");
        printf("Battery saving mode enabled.\n");
    } else if (disable) {
        set_cpu_governor("ondemand");
        adjust_brightness(100);  // Restore brightness to 100%
        enable_services();
        log_status("Battery saving mode disabled.");
        printf("Battery saving mode disabled.\n");
    } else if (status) {
        print_status();
    } else {
        print_usage();  // Fallback to usage message if no valid option
        return 1;
    }

    return 0;
}

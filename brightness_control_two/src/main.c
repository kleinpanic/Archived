#include "brightness.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTION]...\n", prog_name);
    printf("Options:\n");
    printf("  -s, --save                 Save current brightness\n");
    printf("  -r, --restore              Restore saved brightness\n");
    printf("  --set VALUE                Set brightness to VALUE\n");
    printf("  -a, --adjust DIR           Adjust brightness up or down\n");
    printf("  -h, --help                 Display this help and exit\n");
    printf("  -l, --list                 List devices with available brightness controls\n");
    printf("  -q, --quiet                Suppress output\n");
    printf("  -p, --pretend              Do not perform write operations\n");
    printf("  -n, --min-value MIN_VALUE  Set minimum brightness value\n");
    printf("  -e, --exponent EXPONENT    Change percentage curve to exponential\n");
    printf("  -c, --class CLASS          Specify device class\n");
    printf("  -d, --device DEVICE        Specify device name\n");
    printf("  -v, --version              Print version and exit\n");
}

int main(int argc, char *argv[]) {
    int opt;
    static struct option long_options[] = {
        {"save", no_argument, NULL, 's'},
        {"restore", no_argument, NULL, 'r'},
        {"set", required_argument, NULL, 0},
        {"adjust", required_argument, NULL, 'a'},
        {"help", no_argument, NULL, 'h'},
        {"list", no_argument, NULL, 'l'},
        {"quiet", no_argument, NULL, 'q'},
        {"pretend", no_argument, NULL, 'p'},
        {"min-value", required_argument, NULL, 'n'},
        {"exponent", required_argument, NULL, 'e'},
        {"class", required_argument, NULL, 'c'},
        {"device", required_argument, NULL, 'd'},
        {"version", no_argument, NULL, 'v'},
        {NULL, 0, NULL, 0}
    };

    while ((opt = getopt_long(argc, argv, "sra:hlqpne:c:d:v", long_options, NULL)) != -1) {
        switch (opt) {
            case 's':
                save_brightness();
                break;
            case 'r':
                restore_brightness();
                break;
            case 0: // --set
                if (optarg) {
                    int value = atoi(optarg);
                    set_brightness(value, 0);
                }
                break;
            case 'a':
                if (optarg) {
                    adjust_brightness(optarg);
                }
                break;
            case 'h':
                print_usage(argv[0]);
                return EXIT_SUCCESS;
            case 'l':
                list_devices();
                break;
            case 'q':
                enable_quiet_mode();
                break;
            case 'p':
                enable_pretend_mode();
                break;
            case 'n':
                if (optarg) {
                    int min_value = atoi(optarg);
                    set_min_value(min_value);
                }
                break;
            case 'e':
                if (optarg) {
                    float exponent = atof(optarg);
                    set_exponent(exponent);
                }
                break;
            case 'c':
                if (optarg) {
                    set_class(optarg);
                }
                break;
            case 'd':
                if (optarg) {
                    set_device(optarg);
                }
                break;
            case 'v':
                printf("Version 1.0\n");
                return EXIT_SUCCESS;
            default:
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (argc == 1) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

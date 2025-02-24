#include "brightness.h"
#include "display.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Include string.h for strdup

static int min_value = 0;
static float exponent = 1.0;
static char *device = NULL;
static char *class = NULL;
static int quiet_mode = 0;
static int pretend_mode = 0;

void set_brightness(int value, int raw) {
    if (pretend_mode) {
        if (!quiet_mode) {
            printf("Pretend mode: setting brightness to %d\n", value);
        }
        return;
    }

    FILE *brightness_file = fopen(BRIGHTNESS_PATH, "w");
    if (brightness_file == NULL) {
        perror("Failed to open brightness file");
        exit(EXIT_FAILURE);
    }

    if (!raw) {
        FILE *max_brightness_file = fopen(MAX_BRIGHTNESS_PATH, "r");
        if (max_brightness_file == NULL) {
            perror("Failed to open max brightness file");
            fclose(brightness_file);
            exit(EXIT_FAILURE);
        }

        int max_brightness;
        fscanf(max_brightness_file, "%d", &max_brightness);
        fclose(max_brightness_file);

        value = (value * max_brightness) / 100;
    }

    fprintf(brightness_file, "%d", value);
    fclose(brightness_file);
    display_brightness(value);
}

void adjust_brightness(const char *direction) {
    if (pretend_mode) {
        if (!quiet_mode) {
            printf("Pretend mode: adjusting brightness %s\n", direction);
        }
        return;
    }

    FILE *brightness_file = fopen(BRIGHTNESS_PATH, "r+");
    if (brightness_file == NULL) {
        perror("Failed to open brightness file");
        exit(EXIT_FAILURE);
    }

    int brightness;
    fscanf(brightness_file, "%d", &brightness);

    FILE *max_brightness_file = fopen(MAX_BRIGHTNESS_PATH, "r");
    if (max_brightness_file == NULL) {
        perror("Failed to open max brightness file");
        fclose(brightness_file);
        exit(EXIT_FAILURE);
    }

    int max_brightness;
    fscanf(max_brightness_file, "%d", &max_brightness);
    fclose(max_brightness_file);

    if (strcmp(direction, "up") == 0) {
        brightness += max_brightness / 10;
    } else if (strcmp(direction, "down") == 0) {
        brightness -= max_brightness / 10;
    }

    if (brightness < 0) {
        brightness = 0;
    } else if (brightness > max_brightness) {
        brightness = max_brightness;
    }

    rewind(brightness_file);
    fprintf(brightness_file, "%d", brightness);
    fclose(brightness_file);

    display_brightness(brightness);
}

void save_brightness(void) {
    if (pretend_mode) {
        if (!quiet_mode) {
            printf("Pretend mode: saving brightness\n");
        }
        return;
    }

    FILE *brightness_file = fopen(BRIGHTNESS_PATH, "r");
    if (brightness_file == NULL) {
        perror("Failed to open brightness file");
        exit(EXIT_FAILURE);
    }

    int brightness;
    fscanf(brightness_file, "%d", &brightness);
    fclose(brightness_file);

    FILE *save_file = fopen("/tmp/brightnessctl_save", "w");
    if (save_file == NULL) {
        perror("Failed to open save file");
        exit(EXIT_FAILURE);
    }

    fprintf(save_file, "%d", brightness);
    fclose(save_file);

    if (!quiet_mode) {
        printf("Brightness saved: %d\n", brightness);
    }
}

void restore_brightness(void) {
    if (pretend_mode) {
        if (!quiet_mode) {
            printf("Pretend mode: restoring brightness\n");
        }
        return;
    }

    FILE *save_file = fopen("/tmp/brightnessctl_save", "r");
    if (save_file == NULL) {
        perror("Failed to open save file");
        exit(EXIT_FAILURE);
    }

    int brightness;
    fscanf(save_file, "%d", &brightness);
    fclose(save_file);

    set_brightness(brightness, 1);

    if (!quiet_mode) {
        printf("Brightness restored: %d\n", brightness);
    }
}

void list_devices(void) {
    printf("Listing available devices...\n");
}

void print_info(void) {
    printf("Printing device info...\n");
}

void set_min_value(int value) {
    min_value = value;
}

void set_exponent(float exp) {
    exponent = exp;
}

void set_device(char *device_name) {
    device = strdup(device_name);
}

void set_class(char *class_name) {
    class = strdup(class_name);
}

void enable_quiet_mode(void) {
    quiet_mode = 1;
}

void enable_pretend_mode(void) {
    pretend_mode = 1;
}

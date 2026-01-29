#include <stdio.h>
#include <stdlib.h>
#include "brightness_control.h"

int adjust_brightness(int value) {
    FILE *brightness = fopen("/sys/class/backlight/intel_backlight/brightness", "w");
    if (brightness) {
        fprintf(brightness, "%d", value);
        fclose(brightness);
        return 0;
    } else {
        perror("Failed to adjust brightness");
        return 1;
    }
}


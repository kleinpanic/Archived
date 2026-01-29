#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "brightness.h"

void adjust_brightness() {
    FILE *brightness_file;
    int brightness;
    char path[] = "/sys/class/backlight/intel_backlight/brightness";
    char max_brightness_path[] = "/sys/class/backlight/intel_backlight/max_brightness";
    int max_brightness;

    brightness_file = fopen(path, "r+");
    if (brightness_file == NULL) {
        mvprintw(4, 0, "Error: %s", strerror(errno));
        refresh();
        return;
    }

    FILE *max_brightness_file = fopen(max_brightness_path, "r");
    if (max_brightness_file == NULL) {
        mvprintw(4, 0, "Error: %s", strerror(errno));
        fclose(brightness_file);
        refresh();
        return;
    }
    fscanf(max_brightness_file, "%d", &max_brightness);
    fclose(max_brightness_file);

    fscanf(brightness_file, "%d", &brightness);
    mvprintw(4, 0, "Current Brightness: %d", brightness);
    mvprintw(5, 0, "Max Brightness: %d", max_brightness);
    mvprintw(6, 0, "Enter new brightness (0-%d): ", max_brightness);

    refresh();
    echo();
    scanw("%d", &brightness);
    noecho();

    if (brightness < 0 || brightness > max_brightness) {
        mvprintw(7, 0, "Invalid brightness value");
    } else {
        rewind(brightness_file);
        fprintf(brightness_file, "%d", brightness);
        fflush(brightness_file);
        mvprintw(7, 0, "Brightness set to %d", brightness);
    }

    fclose(brightness_file);
    refresh();
    getch();
}

#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "color_temperature.h"

void adjust_color_temperature() {
    mvprintw(4, 0, "Enter the desired color temperature (in Kelvin, e.g., 6500): ");
    refresh();

    echo();
    int color_temp;
    scanw("%d", &color_temp);
    noecho();

    char command[128];
    snprintf(command, sizeof(command), "xcalib -co %d", color_temp);

    int result = system(command);
    if (result == -1) {
        mvprintw(6, 0, "Failed to set color temperature");
    } else {
        mvprintw(6, 0, "Color temperature set to %dK", color_temp);
    }
    refresh();
    getch();
}

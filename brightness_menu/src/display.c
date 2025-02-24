#include <ncurses.h>
#include <stdlib.h>
#include "display.h"
#include "brightness.h"
#include "resolution.h"
#include "refresh_rate.h"
#include "color_temperature.h"

void display_main_screen() {
    int option;
    mvprintw(0, 0, "Select an option:");
    mvprintw(1, 0, "1. Screen Brightness");
    mvprintw(2, 0, "2. Screen Resolution");
    mvprintw(3, 0, "3. Refresh Rate");
    mvprintw(4, 0, "4. Color Temperature");
    mvprintw(5, 0, "5. Exit");

    refresh();
    option = getch() - '0';

    handle_option(option);
}

void handle_option(int option) {
    switch(option) {
        case 1:
            adjust_brightness();
            break;
        case 2:
            adjust_resolution();
            break;
        case 3:
            adjust_refresh_rate();
            break;
        case 4:
            adjust_color_temperature();
            break;
        case 5:
            endwin();
            exit(0);
        default:
            mvprintw(6, 0, "Invalid option");
            refresh();
            getch();
            break;
    }
    display_main_screen();
}


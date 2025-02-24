#include <ncurses.h>
#include <string.h>
#include <time.h>
#include "calendar.h"
#include "day_view.h"
#include "add_event.h"

int main() {
    int ch, month, year, day = 1;
    time_t now;
    struct tm *local;

    // Open the SQLite database
    sqlite3 *db = open_database();
    if (db == NULL) {
        return 1;  // Exit if the database can't be opened
    }

    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Red color pair for the current time

    // Get current date
    now = time(NULL);
    local = localtime(&now);
    month = local->tm_mon + 1;
    year = local->tm_year + 1900;
    day = local->tm_mday;

    // Main loop
    while ((ch = getch()) != 'q') {
        int days_in_current_month = days_in_month(month, year);
        bool redraw = false;

        switch (ch) {
            case KEY_LEFT:
            case 'j':
                if (day > 1) {
                    day--;
                    redraw = true;
                }
                break;
            case KEY_RIGHT:
            case 'k':
                if (day < days_in_current_month) {
                    day++;
                    redraw = true;
                }
                break;
            case 'a':  // 'a' for "Add Event"
                open_add_event_tui(db);  // Pass the database connection here
                redraw = true;
                break;
            case '\n':  // Enter key to open the day view
                draw_day_view(&day, &month, &year);
                redraw = true;
                break;
            case 'h':  // 'h' for previous month
            case KEY_SLEFT:  // Left Shift for previous month
                if (month > 1) {
                    month--;
                } else {
                    month = 12;
                    year--;
                }
                // Adjust the day if it exceeds the number of days in the new month
                if (day > days_in_month(month, year)) {
                    day = days_in_month(month, year);
                }
                redraw = true;
                break;
            case 'l':  // 'l' for next month
            case KEY_SRIGHT:  // Right Shift for next month
                if (month < 12) {
                    month++;
                } else {
                    month = 1;
                    year++;
                }
                // Adjust the day if it exceeds the number of days in the new month
                if (day > days_in_month(month, year)) {
                    day = days_in_month(month, year);
                }
                redraw = true;
                break;
        }

        if (redraw) {
            clear();
            mvprintw(0, 0, "Calendar View - Press 'a' to Add Event");
            draw_calendar(month, year, day);
            refresh();
        }
    }

    // Close the SQLite database connection
    sqlite3_close(db);

    // End ncurses mode
    endwin();
    return 0;
}

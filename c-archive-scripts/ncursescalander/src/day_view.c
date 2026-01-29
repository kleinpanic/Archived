#include "day_view.h"
#include <ncurses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TITLE_LEN 50

typedef struct {
    char title[MAX_TITLE_LEN];
    int start_hour;
    int start_minute;
    int end_hour;
    int end_minute;
    int event_day;
    int event_month;
    int event_year;
} DisplayEvent;

void load_events(DisplayEvent *events, int *count, int day, int month, int year) {
    FILE *file = fopen("events.txt", "r");
    if (file) {
        char line[256];
        int current_day, current_month, current_year;
        while (fgets(line, sizeof(line), file)) {
            if (sscanf(line, "Date: %d-%d-%d", &current_day, &current_month, &current_year) == 3) {
                if (current_day == day && current_month == month && current_year == year) {
                    fgets(line, sizeof(line), file);
                    sscanf(line, "Start: %02d:%02d", &events[*count].start_hour, &events[*count].start_minute);
                    fgets(line, sizeof(line), file);
                    sscanf(line, "End: %02d:%02d", &events[*count].end_hour, &events[*count].end_minute);
                    fgets(line, sizeof(line), file);
                    sscanf(line, "Title: %49[^\n]", events[*count].title);
                    events[*count].event_day = current_day;
                    events[*count].event_month = current_month;
                    events[*count].event_year = current_year;
                    (*count)++;
                }
            }
        }
        fclose(file);
    }
}

void draw_day_view(int *day, int *month, int *year) {
    time_t now;
    struct tm *local;
    int ch;
    bool redraw = true;

    DisplayEvent events[10];
    int event_count = 0;
    load_events(events, &event_count, *day, *month, *year);

    while (true) {
        if (redraw) {
            clear();
            mvprintw(0, 0, "Day View");
            mvprintw(1, 0, "Date: %02d-%02d-%04d", *day, *month, *year);

            // Draw the hours of the day
            for (int i = 0; i < 24; i++) {
                mvprintw(i + 3, 0, "%02d:00 |", i);
                mvhline(i + 3, 8, ' ', COLS - 8);
            }

            // Display events
            for (int i = 0; i < event_count; i++) {
                mvprintw(events[i].start_hour + 3, 10, "%s", events[i].title);
                mvhline(events[i].start_hour + 3, 10 + strlen(events[i].title), '-', COLS - 20);
            }

            now = time(NULL);
            local = localtime(&now);

            if (local->tm_mday == *day && (local->tm_mon + 1) == *month && (local->tm_year + 1900) == *year) {
                int current_hour = local->tm_hour;
                int current_minute = local->tm_min;

                attron(COLOR_PAIR(1));  // Red color
                mvhline(current_hour + 3, 8, '-', COLS - 8);
                mvprintw(current_hour + 3, 0, "%02d:%02d |", current_hour, current_minute);
                attroff(COLOR_PAIR(1));
            }

            refresh();
            redraw = false;
        }

        nodelay(stdscr, TRUE);  // Make getch non-blocking
        ch = getch();

        if (ch == 'q') break;  // Exit to Calendar View
        if (ch == KEY_LEFT || ch == 'j') {
            if (*day > 1) {
                (*day)--;
            } else {
                if (*month > 1) {
                    (*month)--;
                } else {
                    *month = 12;
                    (*year)--;
                }
                *day = 31;  // Simplified for demo purposes
            }
            redraw = true;
        } else if (ch == KEY_RIGHT || ch == 'k') {
            if (*day < 31) {  // Simplified for demo purposes
                (*day)++;
            } else {
                *day = 1;
                if (*month < 12) {
                    (*month)++;
                } else {
                    *month = 1;
                    (*year)++;
                }
            }
            redraw = true;
        }

        napms(1000);
    }

    nodelay(stdscr, FALSE);
}


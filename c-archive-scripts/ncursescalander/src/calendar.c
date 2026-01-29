#include "calendar.h"
#include <ncurses.h>

// Function to determine if a year is a leap year
int is_leap_year(int year) {
    if (year % 4 == 0) {
        if (year % 100 == 0) {
            if (year % 400 == 0)
                return 1;
            else
                return 0;
        } else
            return 1;
    } else
        return 0;
}

// Function to get the number of days in a given month and year
int days_in_month(int month, int year) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            if (is_leap_year(year))
                return 29;
            else
                return 28;
        default:
            return 30; // Fallback, though we shouldn't hit this
    }
}

void draw_calendar(int month, int year, int selected_day) {
    int days = days_in_month(month, year);
    
    // Adjust selected_day if it exceeds the number of days in the month
    if (selected_day > days) {
        selected_day = days;
    }

    mvprintw(1, 0, "Month: %d, Year: %d", month, year);
    mvprintw(2, 0, "Su Mo Tu We Th Fr Sa");

    // Placeholder for calendar drawing
    for (int i = 1; i <= days; i++) {
        if (i == selected_day) {
            attron(A_REVERSE);  // Highlight the selected day
        }
        mvprintw(3 + (i / 7), (i % 7) * 3, "%2d", i);
        if (i == selected_day) {
            attroff(A_REVERSE);
        }
    }
}

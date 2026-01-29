#include <ncurses.h>
#include <string.h>
#include <time.h>

#define MAX_TITLE_LEN 50
#define MAX_CALENDAR_LEN 30
#define MAX_NOTES_LEN 256

void draw_calendar(int month, int year, int selected_day);
void draw_day_view(int *day, int *month, int *year);
void open_add_event_tui();
void open_repeat_tui();

int main() {
    int ch, month, year, day = 1;
    time_t now;
    struct tm *local;

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
                if (day < 31) {  // Simplified for demo purposes
                    day++;
                    redraw = true;
                }
                break;
            case 'a':  // 'a' for "Add Event"
                open_add_event_tui();
                redraw = true;
                break;
            case '\n':  // Enter key to open the day view
                draw_day_view(&day, &month, &year);
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

    // End ncurses mode
    endwin();
    return 0;
}

void draw_calendar(int month, int year, int selected_day) {
    mvprintw(1, 0, "Month: %d, Year: %d", month, year);
    mvprintw(2, 0, "Su Mo Tu We Th Fr Sa");

    // Placeholder for calendar drawing
    for (int i = 1; i <= 30; i++) {  // Simplified example for 30 days
        if (i == selected_day) {
            attron(A_REVERSE);  // Highlight the selected day
        }
        mvprintw(3 + (i / 7), (i % 7) * 3, "%2d", i);
        if (i == selected_day) {
            attroff(A_REVERSE);
        }
    }
}

void draw_day_view(int *day, int *month, int *year) {
    time_t now;
    struct tm *local;
    int ch;
    bool redraw = true;

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

void open_add_event_tui() {
    char title[MAX_TITLE_LEN] = "";
    char calendar[MAX_CALENDAR_LEN] = "";
    char notes[MAX_NOTES_LEN] = "";
    int ch;
    bool all_day = false;
    int start_day = 1, end_day = 1;
    int start_hour = 9, start_minute = 0;
    int end_hour = 10, end_minute = 0;
    int repeat_option = 0;
    char repeat_options[4][20] = {"Never", "Every Day", "Every Week", "Custom"};

    int cursor_pos = 0;

    while (true) {
        clear();
        mvprintw(0, 0, "Add New Event");

        // Title input
        mvprintw(2, 0, "Title: ");
        if (cursor_pos == 0) attron(A_REVERSE);
        mvprintw(2, 7, title);
        if (cursor_pos == 0) attroff(A_REVERSE);

        // All-day toggle
        mvprintw(4, 0, "All Day: ");
        if (cursor_pos == 1) attron(A_REVERSE);
        mvprintw(4, 9, all_day ? "[X]" : "[ ]");
        if (cursor_pos == 1) attroff(A_REVERSE);

        // Start day and time
        mvprintw(6, 0, "Start Day: %02d", start_day);
        mvprintw(7, 0, "Start Time: ");
        if (cursor_pos == 2) attron(A_REVERSE);
        mvprintw(7, 12, "%02d:%02d", start_hour, start_minute);
        if (cursor_pos == 2) attroff(A_REVERSE);

        // End day and time
        mvprintw(9, 0, "End Day: %02d", end_day);
        mvprintw(10, 0, "End Time: ");
        if (cursor_pos == 3) attron(A_REVERSE);
        mvprintw(10, 10, "%02d:%02d", end_hour, end_minute);
        if (cursor_pos == 3) attroff(A_REVERSE);

        // Repeat options
        mvprintw(12, 0, "Repeat: ");
        if (cursor_pos == 4) attron(A_REVERSE);
        mvprintw(12, 8, repeat_options[repeat_option]);
        if (cursor_pos == 4) attroff(A_REVERSE);

        // Calendar name input
        mvprintw(14, 0, "Calendar: ");
        if (cursor_pos == 5) attron(A_REVERSE);
        mvprintw(14, 10, calendar);
        if (cursor_pos == 5) attroff(A_REVERSE);

        // Notes input
        mvprintw(16, 0, "Notes: ");
        if (cursor_pos == 6) attron(A_REVERSE);
        mvprintw(17, 0, notes);
        if (cursor_pos == 6) attroff(A_REVERSE);

        // Instructions
        mvprintw(19, 0, "Press 'q' to cancel, 's' to save");

        refresh();

        ch = getch();
        if (ch == 'q') break;  // Exit without saving
        if (ch == 's') {
            // Here, you would normally save the event
            break;
        }

        // Handle navigation
        if (ch == KEY_DOWN || ch == 'j') {
            cursor_pos = (cursor_pos + 1) % 7;
        } else if (ch == KEY_UP || ch == 'k') {
            cursor_pos = (cursor_pos - 1 + 7) % 7;
        }

        // Handle specific field input
        if (cursor_pos == 0 && ch != KEY_DOWN && ch != KEY_UP && ch != 's' && ch != 'q') {
            int len = strlen(title);
            if (ch == KEY_BACKSPACE || ch == 127) {
                if (len > 0) title[len - 1] = '\0';
            } else if (len < MAX_TITLE_LEN - 1) {
                title[len] = ch;
                title[len + 1] = '\0';
            }
        } else if (cursor_pos == 1 && (ch == ' ' || ch == '\n')) {
            all_day = !all_day;
        } else if (cursor_pos == 2 && (ch == ' ' || ch == '\n')) {
            // Placeholder: Handle start day and time input
        } else if (cursor_pos == 3 && (ch == ' ' || ch == '\n')) {
            // Placeholder: Handle end day and time input
        } else if (cursor_pos == 4 && (ch == ' ' || ch == '\n')) {
            // Open repeat TUI
            open_repeat_tui();
        } else if (cursor_pos == 5 && ch != KEY_DOWN && ch != KEY_UP && ch != 's' && ch != 'q') {
            int len = strlen(calendar);
            if (ch == KEY_BACKSPACE || ch == 127) {
                if (len > 0) calendar[len - 1] = '\0';
            } else if (len < MAX_CALENDAR_LEN - 1) {
                calendar[len] = ch;
                calendar[len + 1] = '\0';
            }
        } else if (cursor_pos == 6 && ch != KEY_DOWN && ch != KEY_UP && ch != 's' && ch != 'q') {
            int len = strlen(notes);
            if (ch == KEY_BACKSPACE || ch == 127) {
                if (len > 0) notes[len - 1] = '\0';
            } else if (len < MAX_NOTES_LEN - 1) {
                notes[len] = ch;
                notes[len + 1] = '\0';
            }
        }
    }
}

void open_repeat_tui() {
    int ch;
    int cursor_pos = 0;

    while (true) {
        clear();
        mvprintw(0, 0, "Repeat Options");

        mvprintw(2, 0, "1. Never");
        if (cursor_pos == 0) attron(A_REVERSE);
        mvprintw(2, 10, "Select");
        if (cursor_pos == 0) attroff(A_REVERSE);

        mvprintw(4, 0, "2. Every Day");
        if (cursor_pos == 1) attron(A_REVERSE);
        mvprintw(4, 10, "Select");
        if (cursor_pos == 1) attroff(A_REVERSE);

        mvprintw(6, 0, "3. Every Week");
        if (cursor_pos == 2) attron(A_REVERSE);
        mvprintw(6, 10, "Select");
        if (cursor_pos == 2) attroff(A_REVERSE);

        mvprintw(8, 0, "4. Custom");
        if (cursor_pos == 3) attron(A_REVERSE);
        mvprintw(8, 10, "Select");
        if (cursor_pos == 3) attroff(A_REVERSE);

        mvprintw(10, 0, "Press 'q' to return");

        refresh();

        ch = getch();
        if (ch == 'q') break;  // Return to the add event TUI
        if (ch == KEY_DOWN || ch == 'j') {
            cursor_pos = (cursor_pos + 1) % 4;
        } else if (ch == KEY_UP || ch == 'k') {
            cursor_pos = (cursor_pos - 1 + 4) % 4;
        } else if (ch == '\n') {
            // Here, you would handle selection logic
            break;  // Return after selection for now
        }
    }
}

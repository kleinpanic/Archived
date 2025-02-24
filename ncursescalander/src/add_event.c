#include "add_event.h"
#include "repeat_tui.h"
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#include <stdio.h>
#include <sqlite3.h>
#include <unistd.h>

// Create and initialize a new event
Event *create_event() {
    Event *event = (Event *)malloc(sizeof(Event));
    if (event) {
        memset(event, 0, sizeof(Event));
        strcpy(event->start_date, "01/01/2000");
        strcpy(event->end_date, "01/01/2000");
        event->start_hour = 9;
        event->start_minute = 0;
        event->end_hour = 10;
        event->end_minute = 0;
        event->repeat_option = 0;
    }
    return event;
}

// Function to open the SQLite database

sqlite3* open_database() {
    char path[PATH_MAX];
    char base_dir[PATH_MAX];
    char db_path[PATH_MAX];

    // Get the current working directory
    if (getcwd(path, sizeof(path)) != NULL) {
        printf("Current working directory: %s\n", path);

        // Go up one directory level to the 'ncursescalander' directory
        strncpy(base_dir, path, sizeof(base_dir));
        dirname(base_dir);
        printf("Base directory (one level up): %s\n", base_dir);

        // Calculate required size
        int required_size = snprintf(NULL, 0, "%s/databases/calendar_database.db", base_dir) + 1;
        if (required_size > (int)sizeof(db_path)) {  // Cast to int for comparison
            fprintf(stderr, "Error: Path length exceeds buffer size.\n");
            return NULL;
        }

        // Construct the final path to the database file
        snprintf(db_path, sizeof(db_path), "%s/databases/calendar_database.db", base_dir);
        printf("Final database path: %s\n", db_path);

        sqlite3 *db;
        if (sqlite3_open(db_path, &db)) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            return NULL;
        } else {
            printf("Opened database successfully\n");
            return db;
        }
    } else {
        perror("getcwd() error");
        return NULL;
    }
}

// Function to save an event to the SQLite database
void save_event_to_db(sqlite3 *db, Event *event) {
    // Set default values if necessary
    if (strlen(event->title) == 0) {
        strcpy(event->title, "Untitled");
    }
    if (strlen(event->calendar) == 0) {
        strcpy(event->calendar, "");
    }
    if (strlen(event->notes) == 0) {
        strcpy(event->notes, "");
    }

    // Prepare SQL statement
    char *sql = "INSERT INTO events (title, all_day, start_date, end_date, start_time, end_time, repeat_option, calendar, notes) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        // Bind values to SQL statement
        sqlite3_bind_text(stmt, 1, event->title, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, event->all_day);
        sqlite3_bind_text(stmt, 3, event->start_date, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, event->end_date, -1, SQLITE_STATIC);

        // Format time as HH:MM
        char start_time[6];
        snprintf(start_time, sizeof(start_time), "%02d:%02d", event->start_hour, event->start_minute);
        sqlite3_bind_text(stmt, 5, start_time, -1, SQLITE_STATIC);

        char end_time[6];
        snprintf(end_time, sizeof(end_time), "%02d:%02d", event->end_hour, event->end_minute);
        sqlite3_bind_text(stmt, 6, end_time, -1, SQLITE_STATIC);

        sqlite3_bind_int(stmt, 7, event->repeat_option);
        sqlite3_bind_text(stmt, 8, event->calendar, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 9, event->notes, -1, SQLITE_STATIC);

        // Execute the SQL statement
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        } else {
            printf("Event saved successfully.\n");
        }
        sqlite3_finalize(stmt);
    } else {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    }
}

// Function to display the Add Event TUI and handle user input
void open_add_event_tui(sqlite3 *db) {
    Event *event = create_event();
    int ch;
    int cursor_pos = 0;
    bool exit = false;

    while (!exit) {
        clear();
        mvprintw(0, 0, "Add New Event");

        // Title input
        mvprintw(2, 0, "Title: ");
        if (cursor_pos == 0) attron(A_REVERSE);
        mvprintw(2, 7, event->title);
        if (cursor_pos == 0) attroff(A_REVERSE);

        // All-day toggle
        mvprintw(4, 0, "All Day: ");
        if (cursor_pos == 1) attron(A_REVERSE);
        mvprintw(4, 9, event->all_day ? "[X]" : "[ ]");
        if (cursor_pos == 1) attroff(A_REVERSE);

        // Start date input
        mvprintw(6, 0, "Start Date: ");
        if (cursor_pos == 2) attron(A_REVERSE);
        mvprintw(6, 12, event->start_date);
        if (cursor_pos == 2) attroff(A_REVERSE);

        // End date input
        mvprintw(8, 0, "End Date: ");
        if (cursor_pos == 3) attron(A_REVERSE);
        mvprintw(8, 10, event->end_date);
        if (cursor_pos == 3) attroff(A_REVERSE);

        // Start time input
        mvprintw(10, 0, "Start Time: ");
        if (cursor_pos == 4) attron(A_REVERSE);
        mvprintw(10, 12, "%02d:%02d", event->start_hour, event->start_minute);
        if (cursor_pos == 4) attroff(A_REVERSE);

        // End time input
        mvprintw(12, 0, "End Time: ");
        if (cursor_pos == 5) attron(A_REVERSE);
        mvprintw(12, 10, "%02d:%02d", event->end_hour, event->end_minute);
        if (cursor_pos == 5) attroff(A_REVERSE);

        // Repeat options
        mvprintw(14, 0, "Repeat: ");
        if (cursor_pos == 6) attron(A_REVERSE);
        mvprintw(14, 8, "Choose..."); // Display the repeat choice interactively
        if (cursor_pos == 6) attroff(A_REVERSE);

        // Calendar name input
        mvprintw(16, 0, "Calendar: ");
        if (cursor_pos == 7) attron(A_REVERSE);
        mvprintw(16, 10, event->calendar);
        if (cursor_pos == 7) attroff(A_REVERSE);

        // Notes input
        mvprintw(18, 0, "Notes: ");
        if (cursor_pos == 8) attron(A_REVERSE);
        mvprintw(19, 0, event->notes);
        if (cursor_pos == 8) attroff(A_REVERSE);

        // Instructions
        mvprintw(21, 0, "Esc: Cancel  Ctrl+S: Save");

        refresh();

        ch = getch();
        switch (ch) {
            case 27:  // ESC to cancel
                exit = true;
                break;
            case 19:  // Ctrl+S to save (^S)
                save_event_to_db(db, event);
                exit = true;  // Return to calendar view after saving
                break;
            case KEY_DOWN:  // Navigate down
                cursor_pos = (cursor_pos + 1) % 9;
                break;
            case KEY_UP:  // Navigate up
                cursor_pos = (cursor_pos - 1 + 9) % 9;
                break;
            case KEY_LEFT:
            case 'j':  // Decrease minute
                if (cursor_pos == 4) {
                    event->start_minute = (event->start_minute > 0) ? event->start_minute - 1 : 59;
                } else if (cursor_pos == 5) {
                    event->end_minute = (event->end_minute > 0) ? event->end_minute - 1 : 59;
                }
                break;
            case KEY_RIGHT:
            case 'k':  // Increase minute
                if (cursor_pos == 4) {
                    event->start_minute = (event->start_minute < 59) ? event->start_minute + 1 : 0;
                } else if (cursor_pos == 5) {
                    event->end_minute = (event->end_minute < 59) ? event->end_minute + 1 : 0;
                }
                break;
            case KEY_SLEFT:
            case 'h':  // Decrease hour (Shift+Left or 'h')
                if (cursor_pos == 4) {
                    event->start_hour = (event->start_hour > 0) ? event->start_hour - 1 : 23;
                } else if (cursor_pos == 5) {
                    event->end_hour = (event->end_hour > 0) ? event->end_hour - 1 : 23;
                }
                break;
            case KEY_SRIGHT:
            case 'l':  // Increase hour (Shift+Right or 'l')
                if (cursor_pos == 4) {
                    event->start_hour = (event->start_hour < 23) ? event->start_hour + 1 : 0;
                } else if (cursor_pos == 5) {
                    event->end_hour = (event->end_hour < 23) ? event->end_hour + 1 : 0;
                }
                break;
            case ' ':
            case '\n':
                if (cursor_pos == 1) {
                    event->all_day = !event->all_day;
                } else if (cursor_pos == 6) {
                    open_repeat_tui();  // Placeholder for opening the repeat TUI
                }
                break;
            default:
                if (cursor_pos == 0 && ch != KEY_DOWN && ch != KEY_UP) {
                    int len = strlen(event->title);
                    if ((ch == KEY_BACKSPACE || ch == 127) && len > 0) {
                        event->title[len - 1] = '\0';
                    } else if (isprint(ch) && len < MAX_TITLE_LEN - 1) {
                        event->title[len] = ch;
                        event->title[len + 1] = '\0';
                    }
                } else if (cursor_pos == 2 && ch != KEY_DOWN && ch != KEY_UP) {
                    int len = strlen(event->start_date);
                    if ((ch == KEY_BACKSPACE || ch == 127) && len > 0) {
                        event->start_date[len - 1] = '\0';
                    } else if (isprint(ch) && len < MAX_DATE_LEN - 1) {
                        event->start_date[len] = ch;
                        event->start_date[len + 1] = '\0';
                    }
                } else if (cursor_pos == 3 && ch != KEY_DOWN && ch != KEY_UP) {
                    int len = strlen(event->end_date);
                    if ((ch == KEY_BACKSPACE || ch == 127) && len > 0) {
                        event->end_date[len - 1] = '\0';
                    } else if (isprint(ch) && len < MAX_DATE_LEN - 1) {
                        event->end_date[len] = ch;
                        event->end_date[len + 1] = '\0';
                    }
                } else if (cursor_pos == 7 && ch != KEY_DOWN && ch != KEY_UP) {
                    int len = strlen(event->calendar);
                    if ((ch == KEY_BACKSPACE || ch == 127) && len > 0) {
                        event->calendar[len - 1] = '\0';
                    } else if (isprint(ch) && len < MAX_CALENDAR_LEN - 1) {
                        event->calendar[len] = ch;
                        event->calendar[len + 1] = '\0';
                    }
                } else if (cursor_pos == 8 && ch != KEY_DOWN && ch != KEY_UP) {
                    int len = strlen(event->notes);
                    if ((ch == KEY_BACKSPACE || ch == 127) && len > 0) {
                        event->notes[len - 1] = '\0';
                    } else if (isprint(ch) && len < MAX_NOTES_LEN - 1) {
                        event->notes[len] = ch;
                        event->notes[len + 1] = '\0';
                    }
                }
                break;
        }
    }

    free(event);  // Free the allocated memory
}

#ifndef ADD_EVENT_H
#define ADD_EVENT_H

#include <sqlite3.h>
#include <stdbool.h>

#define MAX_TITLE_LEN 50
#define MAX_CALENDAR_LEN 30
#define MAX_NOTES_LEN 256
#define MAX_DATE_LEN 50

typedef struct {
    char title[MAX_TITLE_LEN];
    bool all_day;
    char start_date[MAX_DATE_LEN];
    char end_date[MAX_DATE_LEN];
    int start_hour, start_minute;
    int end_hour, end_minute;
    int repeat_option;
    char calendar[MAX_CALENDAR_LEN];
    char notes[MAX_NOTES_LEN];
} Event;

Event *create_event();
void save_event_to_db(sqlite3 *db, Event *event);
sqlite3* open_database();
void open_add_event_tui(sqlite3 *db);

#endif

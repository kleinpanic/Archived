#include <ncurses.h>
#include "repeat_tui.h"

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

        mvprintw(8, 0, "4. Every Other Day");
        if (cursor_pos == 3) attron(A_REVERSE);
        mvprintw(8, 18, "Select");
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


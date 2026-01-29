#include "InputHandler.h"

void InputHandler::handleInput(GridLayout& grid) {
    int ch;
    while ((ch = getch()) != 'q') { // Exit on 'q'
        switch(ch) {
            case 'h': grid.moveSelection(-1, 0); break;
            case 'j': grid.moveSelection(0, 1); break;
            case 'k': grid.moveSelection(0, -1); break;
            case 'l': grid.moveSelection(1, 0); break;
            case '\n': grid.showElementDetails(); break;
            case '?': showHelpScreen(); break;
        }
        grid.displayGrid();
    }
}

void InputHandler::showHelpScreen() {
    clear();
    mvprintw(0, 0, "Help: Use 'h', 'j', 'k', 'l' to navigate, Enter to view details, 'q' to quit.");
    refresh();
    getch(); // Wait for key press
}


#include "ScreenRenderer.h"

void ScreenRenderer::render(GridLayout& grid) {
    initscr();
    start_color();
    curs_set(0); // Hide cursor

    grid.displayGrid();

    InputHandler inputHandler;
    inputHandler.handleInput(grid);

    endwin(); // End ncurses mode
}


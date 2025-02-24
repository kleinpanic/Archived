/*
 * main.c - Entry point of the Terrarium program
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>   /* for system() */
#include <unistd.h>   /* for usleep() */

#include "game.h"

int main(void) {
    GameState gs;

    /* Load a previous save or start fresh. */
    loadGame(&gs);

    /* Setup terminal for non-blocking input, no echo */
    initTermios(0);

    int running = 1;
    while (running) {
        /* Time since the last loop in seconds */
        double elapsedSeconds = getElapsedTime();

        /* Update game logic (oxygen accumulation, etc.) */
        updateGame(&gs, elapsedSeconds);

        /* Render the shelves and the info area */
        renderGame(&gs);

        /* Check user input (non-blocking) */
        if (kbhit()) {
            char c = getch();
            if (c == 'q' || c == 'Q') {
                running = 0;  /* quit */
            } else {
                handleInput(&gs, c);
            }
        }

        /* Control frame/tick rate (~5 times a second) */
        usleep(GAME_TICK_INTERVAL_MS * 1000);

        /* Auto-save each loop (optional) */
        saveGame(&gs);
    }

    /* Cleanup */
    resetTermios();
    clearScreen();
    printf("Thanks for playing Terrarium!\n");

    return 0;
}


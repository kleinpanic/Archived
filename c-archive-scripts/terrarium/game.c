/******************************************************************************
 * game.c
 *
 * Implements core game logic: initialization, update, rendering, input, etc.
 ******************************************************************************/
/*
 * game.c - Contains the implementation of the Terrarium game logic
 */

#define _POSIX_C_SOURCE 200809L

#include "game.h"

/* System / standard headers */
#include <stdio.h>
#include <stdlib.h>   /* for system() */
#include <time.h>     /* for clock_gettime, etc. */
#include <unistd.h>   /* for usleep() */
#include <fcntl.h>
#include <termios.h>
#include <string.h>   /* for strncpy() */

/******************************************************************************
 * Terminal-handling variables/functions (non-blocking input, etc.)
 *****************************************************************************/
static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
    tcgetattr(0, &old);
    current = old;
    current.c_lflag &= ~ICANON;   /* disable buffered i/o */
    if (echo) {
        current.c_lflag |= ECHO;  
    } else {
        current.c_lflag &= ~ECHO; 
    }
    tcsetattr(0, TCSANOW, &current);
}

/* Restore old terminal i/o settings */
void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

/* Check if a key has been pressed (non-blocking) */
int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

/* Read one character (like getch in Windows) */
char getch(void) {
    return getchar();
}

/* Clear the screen (Unix-like) */
void clearScreen(void) {
    system("clear");
}

/******************************************************************************
 * Elapsed Time (we store the old time in a static variable)
 *****************************************************************************/
double getElapsedTime(void) {
    static struct timespec lastTime = {0, 0};
    struct timespec now;

    /* If first call, initialize lastTime */
    if (lastTime.tv_sec == 0 && lastTime.tv_nsec == 0) {
        clock_gettime(CLOCK_MONOTONIC, &now);
        lastTime = now;
        return 0.0;
    }

    /* current time */
    clock_gettime(CLOCK_MONOTONIC, &now);

    double elapsed = (now.tv_sec - lastTime.tv_sec) +
                     (now.tv_nsec - lastTime.tv_nsec) / 1000000000.0;
    lastTime = now;
    return elapsed;
}

/******************************************************************************
 * Game Logic & Rendering
 *****************************************************************************/

/* Forward-declarations (private to this file) */
static double calcTotalProduction(const GameState *gs);
static void buyPlant(GameState *gs);

/* initGame: brand-new game (if load fails or for fresh start) */
void initGame(GameState *gs) {
    gs->oxygen = 0.0;
    gs->plantCost = BASE_PLANT_COST;
    gs->plantCount = 0;

    /* The 40 plants (names). Adjust as you wish. */
    const char* plantNames[MAX_PLANTS] = {
        "Snake Plant", "Aloe Vera", "Rubber fig", "Maidenhair fern", "Zebra Plant",
        "Jade plant", "Yucca", "Chain of Hearts", "ZZ plant", "Moonstones",
        "Chinese Money plant", "String of pearls.", "Air plant", "African milk tree", 
        "pine bonsai", "Lotus", "Heart fern", "Corkscrew rush", "Weeping fig", "Corkscrew albuca",
        "Fiddle leaf fig", "Mikado", "Kebab bush", "Dwarf Papyrus", "Hobbit Crassula",
        "Bunny ear cactus", "ghost echoversia", "chinese lantern", "ginseng ficus", "venus flytrap",
        "Flamingo flower", "Japanese maple bonsai", "Fshbone cactus", "Paddle plant", 
        "Donkey's tail", "Common ivy", "Chinese Crassula", "Blue Chalksticks", "Angel's tears", "White clover"
    };

    for(int i = 0; i < MAX_PLANTS; i++) {
        strncpy(gs->plants[i].name, plantNames[i], sizeof(gs->plants[i].name) - 1);
        gs->plants[i].name[sizeof(gs->plants[i].name)-1] = '\0';
        gs->plants[i].active = 0;
        gs->plants[i].productionRate = 0.0;
    }

    /* Start with the first plant active, if desired */
    gs->plants[0].active = 1;
    gs->plants[0].productionRate = BASE_OXYGEN_PER_SECOND;
    gs->plantCount = 1;
}

/* loadGame: tries to read from a file. If it fails, calls initGame(). */
void loadGame(GameState *gs) {
    FILE *f = fopen("terrarium_save.dat", "rb");
    if(!f) {
        initGame(gs);
        return;
    }
    if(fread(gs, sizeof(GameState), 1, f) != 1) {
        fclose(f);
        initGame(gs);
        return;
    }
    fclose(f);

    /* Basic sanity check */
    if(gs->plantCount < 1) {
        initGame(gs);
    }
}

/* saveGame: writes the current game state to a file. */
void saveGame(const GameState *gs) {
    FILE *f = fopen("terrarium_save.dat", "wb");
    if(!f) return;  
    fwrite(gs, sizeof(GameState), 1, f);
    fclose(f);
}

/* updateGame: called each loop to add oxygen from production rate. */
void updateGame(GameState *gs, double elapsedSeconds) {
    double totalProd = calcTotalProduction(gs);
    gs->oxygen += totalProd * elapsedSeconds;
}

/* renderGame: draws shelves + plants in top 3/4, info in bottom 1/4. */
void renderGame(const GameState *gs) {
    clearScreen();

    printf("========================================\n");
    printf("             T E R R A R I U M         \n");
    printf("========================================\n\n");

    /*
     * We have 5 shelves (SHELF_COUNT=5). 
     * Each shelf displays 8 plants => total 40 plants (PLANTS_PER_SHELF=8).
     */
    int plantIdx = 0;
    for(int shelf = 0; shelf < SHELF_COUNT; shelf++) {
        for(int slot = 0; slot < PLANTS_PER_SHELF; slot++) {
            if(plantIdx < MAX_PLANTS && gs->plants[plantIdx].active) {
                /* Print the plant name (20 chars padded, e.g.) */
                printf("[%2d] %-20s | ", plantIdx+1, gs->plants[plantIdx].name);
            } else {
                printf("[   ] %-20s | ", "(empty)");
            }
            plantIdx++;
        }
        printf("\n--------------------------------------------------------\n");
    }

    /* BOTTOM 1/4: info, oxygen, etc. */
    printf("\nOxygen: %.2f O2\n", gs->oxygen);
    printf("Total Production: %.3f O2/s\n", calcTotalProduction(gs));
    printf("Next Plant Cost: %.2f O2\n\n", gs->plantCost);

    printf("[SPACE] Tap the first plant for +%.1f O2 (if active)\n", TAP_BONUS_OXYGEN);
    printf("[b] Buy a new plant\n");
    printf("[q] Quit\n\n");
}

/* handleInput: user keystrokes (tap or buy, etc.) */
void handleInput(GameState *gs, char input) {
    switch(input) {
        case ' ':
            /* Tap only works on the first plant if active */
            if(gs->plants[0].active) {
                gs->oxygen += TAP_BONUS_OXYGEN;
            }
            break;
        case 'b':
        case 'B':
            buyPlant(gs);
            break;
        default:
            break;
    }
}

/* Internal helper: calculates sum of production rates */
static double calcTotalProduction(const GameState *gs) {
    double total = 0.0;
    for(int i = 0; i < MAX_PLANTS; i++) {
        if(gs->plants[i].active) {
            total += gs->plants[i].productionRate;
        }
    }
    return total;
}

/* Internal helper: buy the next plant if we can afford it */
static void buyPlant(GameState *gs) {
    if(gs->plantCount >= MAX_PLANTS) {
        return;  /* can't buy more than 40 */
    }
    if(gs->oxygen >= gs->plantCost) {
        gs->oxygen -= gs->plantCost;
        /* activate next plant in line */
        int index = gs->plantCount; 
        gs->plants[index].active = 1;
        gs->plants[index].productionRate = BASE_OXYGEN_PER_SECOND;  /* or custom logic */
        gs->plantCount++;

        /* Increase cost for next time */
        gs->plantCost *= COST_MULTIPLIER;
    }
}


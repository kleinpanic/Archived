/******************************************************************************
 * Terrarium - An ASCII-based terminal idle game in C
 *
 *  - Top 3/4 of the screen: shelves to visualize plants with their names.
 *  - Bottom 1/4 of the screen: oxygen total, controls, etc.
 *  - Tapping (space bar) works only for the first plant to give bonus oxygen.
 *  - Press 'b' to buy new plants, which appear on the next available shelf.
 *  - Press 'q' to quit.
 *  - Auto-saves and auto-loads from "terrarium_save.dat".
 *
 * Compile (Linux/Unix):
 *    gcc terrarium.c -o terrarium -lm
 *
 * Run:
 *    ./terrarium
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

/******************************************************************************
 * Platform/Terminal-Handling Code (Non-Blocking Input, Clear Screen, etc.)
 *****************************************************************************/

static struct termios old, current;

/* Initialize new terminal i/o settings */
void initTermios(int echo) {
    tcgetattr(0, &old);      /* grab old terminal i/o settings */
    current = old;           
    current.c_lflag &= ~ICANON;  /* disable buffered i/o */
    if (echo) {
        current.c_lflag |= ECHO;  /* set echo mode */
    } else {
        current.c_lflag &= ~ECHO; /* set no echo mode */
    }
    tcsetattr(0, TCSANOW, &current);  /* apply the new settings */
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

/* Read one character without echo */
char getch() {
    return getchar();
}

/* Clear the screen (Unix-like) */
void clearScreen() {
    system("clear");
}

/******************************************************************************
 * Game Constants
 *****************************************************************************/

#define MAX_PLANTS         10      /* Max number of plants we can display */
#define SHELF_COUNT        5       /* How many shelves to visualize       */
#define GAME_TICK_INTERVAL_MS 200  /* Game loop tick interval in ms       */

#define BASE_OXYGEN_PER_SECOND 0.1
#define TAP_BONUS_OXYGEN       1.0
#define BASE_PLANT_COST        10.0
#define COST_MULTIPLIER        1.2

/* Save file name */
#define SAVE_FILE "terrarium_save.dat"

/******************************************************************************
 * Data Structures
 *****************************************************************************/

/* Each Plant has a name and a productionRate. We could expand this in the future. */
typedef struct {
    char  name[32];
    int   active;           /* 1 if this plant is purchased and displayed, 0 otherwise */
    double productionRate;  /* Oxygen production contribution from this plant */
} Plant;

typedef struct {
    double oxygen;              /* Current total oxygen */
    double plantCost;           /* Cost to buy the next plant */
    Plant  plants[MAX_PLANTS];  /* Our plants (0th is the "first" plant that can be tapped) */
    int    plantCount;          /* How many plants are active/purchased */
} GameState;

/******************************************************************************
 * Function Declarations
 *****************************************************************************/

void initGame(GameState *gs);
void loadGame(GameState *gs);
void saveGame(const GameState *gs);

void updateGame(GameState *gs, double elapsedSeconds);
void renderGame(const GameState *gs);

void handleInput(GameState *gs, char input);
void buyPlant(GameState *gs);

double calcTotalProduction(const GameState *gs);
double getElapsedTime(struct timespec *lastTick);

/******************************************************************************
 * Main Function
 *****************************************************************************/

int main(void) {
    GameState gs;

    /* Attempt to load a previous save. If no save file is found, initialize. */
    loadGame(&gs);

    /* Terminal setup for non-blocking input */
    initTermios(0);  // 0 = no echo

    /* Timing for the game loop */
    struct timespec lastTick;
    clock_gettime(CLOCK_MONOTONIC, &lastTick);

    int running = 1;
    while(running) {
        /* 1) Compute elapsed time */
        double elapsedSeconds = getElapsedTime(&lastTick);

        /* 2) Update game logic */
        updateGame(&gs, elapsedSeconds);

        /* 3) Render the game */
        clearScreen();
        renderGame(&gs);

        /* 4) Check user input (non-blocking) */
        if(kbhit()) {
            char input = getch();
            if(input == 'q' || input == 'Q') {
                running = 0;
            } else {
                handleInput(&gs, input);
            }
        }

        /* 5) Sleep to control game speed */
        usleep(GAME_TICK_INTERVAL_MS * 1000);

        /* 6) Auto-save occasionally (for example, every loop).
         *    You could also do it less frequently to avoid disk writes. */
        saveGame(&gs);
    }

    /* Cleanup */
    resetTermios();
    clearScreen();
    printf("Thanks for playing Terrarium!\n");
    return 0;
}

/******************************************************************************
 * Initialize Game State (default if no save is found)
 *****************************************************************************/
void initGame(GameState *gs) {
    gs->oxygen = 0.0;
    gs->plantCost = BASE_PLANT_COST;

    /* Initialize all plants as inactive, except the first one. */
    for(int i = 0; i < MAX_PLANTS; i++){
        snprintf(gs->plants[i].name, sizeof(gs->plants[i].name), "Plant #%d", i+1);
        gs->plants[i].active = 0;
        gs->plants[i].productionRate = 0.0;
    }

    /* Start with one plant active (the first one) */
    gs->plants[0].active = 1;
    gs->plants[0].productionRate = BASE_OXYGEN_PER_SECOND;
    gs->plantCount = 1;
}

/******************************************************************************
 * Load Game State from SAVE_FILE (if it exists). Otherwise, init fresh.
 *****************************************************************************/
void loadGame(GameState *gs) {
    FILE *f = fopen(SAVE_FILE, "rb");
    if(!f) {
        /* No save found, start a new game */
        initGame(gs);
        return;
    }

    /* Read the saved struct. For a real game, you'd want version checks, etc. */
    fread(gs, sizeof(GameState), 1, f);
    fclose(f);

    /* Sanity check that at least one plant is active. If not, re-init. */
    if(gs->plantCount < 1) {
        initGame(gs);
    }
}

/******************************************************************************
 * Save Game State to SAVE_FILE
 *****************************************************************************/
void saveGame(const GameState *gs) {
    FILE *f = fopen(SAVE_FILE, "wb");
    if(!f) {
        return; /* if we can't open it, just skip saving */
    }
    fwrite(gs, sizeof(GameState), 1, f);
    fclose(f);
}

/******************************************************************************
 * Update Game State
 *  - Accumulate oxygen based on the total production rate * elapsed time
 *****************************************************************************/
void updateGame(GameState *gs, double elapsedSeconds) {
    double totalProd = calcTotalProduction(gs);
    gs->oxygen += totalProd * elapsedSeconds;
}

/******************************************************************************
 * Render/Draw the game to the terminal
 *
 *  We’ll reserve the top 3/4 of the screen for shelves (with ASCII pots/plants),
 *  and the bottom 1/4 for stats, cost, etc.
 *
 *  For simplicity, let's assume a typical 24- or 25-line terminal height.
 *  We'll do 15-16 lines for shelves, then ~8-9 lines for the bottom portion.
 *
 *****************************************************************************/
void renderGame(const GameState *gs) {
    /* We'll define how many lines we want for shelves, ignoring actual
       terminal height detection for simplicity. Adjust as you like. */
    const int shelfLines = 15;

    /* Each shelf is represented by two lines: 
         1) The plant pot / name 
         2) The "--------" shelf line
       We'll show up to SHELF_COUNT shelves from top to bottom.
    */
    int shelfIndex = 0;
    int plantIndex = 0;

    /* Print the top banner */
    printf("========================================\n");
    printf("          T E R R A R I U M            \n");
    printf("========================================\n\n");

    /* We have SHELF_COUNT shelves to display. Let’s map each shelf to a plant index if available. */
    for(shelfIndex = 0; shelfIndex < SHELF_COUNT; shelfIndex++) {
        /* If there's a plant for this shelfIndex, show it. 
           We'll just go in the order plants are purchased (0,1,2,...) 
           and display them as long as they are active, up to the number of shelves. */
        if(plantIndex < MAX_PLANTS && gs->plants[plantIndex].active) {
            /* Show an ASCII pot and the plant’s name */
            printf("   [%s]\n", gs->plants[plantIndex].name);
            printf("   ----------------------  (shelf #%d)\n", shelfIndex+1);
            plantIndex++;
        } else {
            /* Empty shelf */
            printf("   [empty]\n");
            printf("   ----------------------  (shelf #%d)\n", shelfIndex+1);
        }
    }

    /* Now we display the bottom portion (the UI area). We'll show the last 1/4 lines. */
    /* Let's just put a few blank lines to ensure separation. */
    printf("\n");

    /* Bottom section with info and controls */
    printf("Oxygen: %.2f O2\n", gs->oxygen);
    printf("Total Production Rate: %.3f O2/sec\n", calcTotalProduction(gs));
    printf("Next Plant Cost: %.2f O2\n\n", gs->plantCost);

    printf("[SPACE] Tap the first plant for +%.1f O2 (only if the first plant exists)\n", TAP_BONUS_OXYGEN);
    printf("[b] Buy a new plant\n");
    printf("[q] Quit\n");
    printf("\n");
}

/******************************************************************************
 * Handle User Input
 *****************************************************************************/
void handleInput(GameState *gs, char input) {
    switch(input) {
        case ' ':
            /* Space bar: Tapping only affects the first plant if active. */
            if(gs->plants[0].active) {
                gs->oxygen += TAP_BONUS_OXYGEN;
            }
            break;
        case 'b':
        case 'B':
            /* Buy a new plant if possible */
            buyPlant(gs);
            break;
        default:
            /* ignore other keys */
            break;
    }
}

/******************************************************************************
 * Buy a new plant (if we can afford it and haven't reached MAX_PLANTS)
 *****************************************************************************/
void buyPlant(GameState *gs) {
    if(gs->plantCount >= MAX_PLANTS) {
        /* No more plants can be bought */
        return;
    }

    /* Check if we have enough oxygen for the next plant */
    if(gs->oxygen >= gs->plantCost) {
        gs->oxygen -= gs->plantCost;

        /* Activate the next plant and set its production rate */
        int index = gs->plantCount;  /* next available slot */
        gs->plants[index].active = 1;
        /* For demonstration: each new plant has the same base production, 
           or you could do something fancier. */
        gs->plants[index].productionRate = BASE_OXYGEN_PER_SECOND;

        gs->plantCount++;

        /* Increase the cost for the next plant */
        gs->plantCost *= COST_MULTIPLIER;
    }
}

/******************************************************************************
 * Calculate the Total Production from all active plants
 *****************************************************************************/
double calcTotalProduction(const GameState *gs) {
    double total = 0.0;
    for(int i = 0; i < MAX_PLANTS; i++) {
        if(gs->plants[i].active) {
            total += gs->plants[i].productionRate;
        }
    }
    return total;
}

/******************************************************************************
 * Get elapsed time in seconds since the last tick
 *****************************************************************************/
double getElapsedTime(struct timespec *lastTick) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    double elapsed = (now.tv_sec - lastTick->tv_sec)
                   + (now.tv_nsec - lastTick->tv_nsec) / 1000000000.0;
    *lastTick = now;
    return elapsed;
}


/******************************************************************************
 * game.h
 * 
 * Exposes the core game logic and data.
 ******************************************************************************/
#ifndef GAME_H
#define GAME_H

/* 
 * This header declares functions/data for the Terrarium game.
 * Include it from both main.c and game.c.
 */

#define MAX_PLANTS          40
#define SHELF_COUNT         5      /* We'll display 5 shelves total */
#define PLANTS_PER_SHELF    8      /* 8 plants per shelf => 40 plants total */
#define GAME_TICK_INTERVAL_MS 200  /* Loop tick (ms) */

#define BASE_OXYGEN_PER_SECOND 0.1
#define TAP_BONUS_OXYGEN       1.0
#define BASE_PLANT_COST        10.0
#define COST_MULTIPLIER        1.2

/*
 * Data Structures
 */

typedef struct {
    char  name[64];         /* e.g. "Snake Plant" */
    int   active;           /* 1 if purchased/visible, 0 if not */
    double productionRate;  /* O2 per second contributed by this plant */
} Plant;

/* Main Game State container */
typedef struct {
    double oxygen;               /* Current total oxygen currency */
    double plantCost;            /* Cost to buy the next plant */
    Plant  plants[MAX_PLANTS];   /* All 40 plants */
    int    plantCount;           /* How many plants are active */
} GameState;

/*
 * Function Prototypes
 */

/* Game lifecycle */
void initGame(GameState *gs);
void loadGame(GameState *gs);
void saveGame(const GameState *gs);

/* Game logic */
void updateGame(GameState *gs, double elapsedSeconds);
void renderGame(const GameState *gs);
void handleInput(GameState *gs, char input);

/* Timing: returns elapsed seconds since last call */
double getElapsedTime(void);

/* Terminal / I/O helpers */
void initTermios(int echo);
void resetTermios(void);
int  kbhit(void);
char getch(void);
void clearScreen(void);

#endif /* GAME_H */


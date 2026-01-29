#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "GridLayout.h"

class InputHandler {
public:
    void handleInput(GridLayout& grid);

private:
    void showHelpScreen();
};

#endif // INPUTHANDLER_H


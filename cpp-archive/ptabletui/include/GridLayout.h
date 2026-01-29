#ifndef GRIDLAYOUT_H
#define GRIDLAYOUT_H

#include <vector>
#include <ncurses.h>
#include "Element.h"

enum ColorPairs {
    METAL = 1,
    NONMETAL,
    METALLOID,
    GRAYSCALE
};

class GridLayout {
private:
    std::vector<Element> elements;
    int selectedElement;
    
public:
    GridLayout(const std::vector<Element>& elems);

    void displayGrid();
    void moveSelection(int dx, int dy);
    void showElementDetails();

private:
    int determineColor(const Element& element);
    void displayElement(int index, int colorPair);
    int calculateRow(int index);
    int calculateColumn(int index);
};

#endif // GRIDLAYOUT_H


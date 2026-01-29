#include "GridLayout.h"

GridLayout::GridLayout(const std::vector<Element>& elems) : elements(elems), selectedElement(0) {
    // Initialize colors
    init_pair(METAL, COLOR_YELLOW, COLOR_BLACK);
    init_pair(NONMETAL, COLOR_GREEN, COLOR_BLACK);
    init_pair(METALLOID, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(GRAYSCALE, COLOR_WHITE, COLOR_BLACK);
}

void GridLayout::displayGrid() {
    clear();
    for (size_t i = 0; i < elements.size(); ++i) {
        int colorPair = determineColor(elements[i]);
        displayElement(i, colorPair);
    }
    refresh();
}

void GridLayout::moveSelection(int dx, int dy) {
    // Implement movement logic based on grid layout
}

void GridLayout::showElementDetails() {
    // Implement detailed information display
}

int GridLayout::determineColor(const Element& element) {
    // Logic to determine color based on element type
    return METAL; // Example
}

void GridLayout::displayElement(int index, int colorPair) {
    int row = calculateRow(index);
    int col = calculateColumn(index);
    attron(COLOR_PAIR(colorPair));
    mvprintw(row, col, "%d", elements[index].number);
    mvprintw(row + 1, col, "%s", elements[index].symbol.c_str());
    if (index == selectedElement) {
        // Highlight selected element
        mvchgat(row, col, -1, A_REVERSE, colorPair, NULL);
        mvchgat(row + 1, col, -1, A_REVERSE, colorPair, NULL);
    }
    attroff(COLOR_PAIR(colorPair));
}

int GridLayout::calculateRow(int index) {
    // Map index to grid row
    return index / 18 * 2; // Example
}

int GridLayout::calculateColumn(int index) {
    // Map index to grid column
    return (index % 18) * 4; // Example
}


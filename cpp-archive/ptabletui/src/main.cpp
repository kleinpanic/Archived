#include "Element.cpp"
#include "GridLayout.cpp"
#include "ScreenRenderer.cpp"

std::vector<Element> initializeElements() {
    std::vector<Element> elements;
    // Initialize all elements (only a few are shown here)
    elements.push_back(Element(1, "H", "Hydrogen", 1.008, -259.14, -252.87, 72.8, 1, 2.20, 75.0, 10.0));
    elements.push_back(Element(2, "He", "Helium", 4.0026, -272.20, -268.93, 0.0, 1, 0.0, 23.0, 0.0));
    // Add other elements...

    return elements;
}

int main() {
    std::vector<Element> elements = initializeElements();
    GridLayout grid(elements);
    ScreenRenderer renderer;
    renderer.render(grid);

    return 0;
}


#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>

class Element {
public:
    int number;
    std::string symbol;
    std::string name;
    float atomicWeight;
    float meltingPoint;
    float boilingPoint;
    float electronAffinity;
    int energyLevels;
    float electronegativity;
    float abundanceUniverse;
    float abundanceHuman;

    Element(int num, const std::string& sym, const std::string& nm, float atomicWt, float meltPt, 
            float boilPt, float elecAff, int energyLvls, float elecNeg, float abundUniv, float abundHum);
};

#endif // ELEMENT_H


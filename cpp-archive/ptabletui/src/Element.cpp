#include "Element.h"

Element::Element(int num, const std::string& sym, const std::string& nm, float atomicWt, float meltPt, 
                 float boilPt, float elecAff, int energyLvls, float elecNeg, float abundUniv, float abundHum)
    : number(num), symbol(sym), name(nm), atomicWeight(atomicWt), meltingPoint(meltPt), 
      boilingPoint(boilPt), electronAffinity(elecAff), energyLevels(energyLvls),
      electronegativity(elecNeg), abundanceUniverse(abundUniv), abundanceHuman(abundHum) {}


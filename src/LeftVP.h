//
// Created by tgsp on 4/2/2024.
//

#ifndef TGSOUTHAHW4_LEFTVP_H
#define TGSOUTHAHW4_LEFTVP_H


#include <string>
#include "Coord.h"

class LeftVP {
    std::string debug_strings[60];
    Coord bkgColor;
    Coord textColor;


public:
    LeftVP(int lVportW, int lVportH, Coord bkgColor, Coord textColor);

    int addDebugString(int line, std::string inString);

    int lVportW = 300;
    int lVportH = 900;

    void drawViewport();

    LeftVP() = default;

    std::string* getDbgStrPtr() {
        return debug_strings;
    }
};



#endif //TGSOUTHAHW4_LEFTVP_H

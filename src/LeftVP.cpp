//
// Created by tgsp on 4/2/2024.
//

#include "LeftVP.h"

#include <iostream>

int LeftVP::addDebugString(int line, std::string inString) {
    if (line < 0 || line > 59) {
        return -1;
    }
    debug_strings[line] = inString;
    return 0;
}

LeftVP::LeftVP(int lVportW, int lVportH, Coord bkgColor, Coord textColor) {
    this->lVportW = lVportW;
    this->lVportH = lVportH;
    this->bkgColor = bkgColor;
    this->textColor = textColor;
}

void LeftVP::drawViewport() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, lVportW, lVportH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, lVportW, 0, lVportH, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //draw blue bounding box around the viewport

    glClearColor(bkgColor.X, bkgColor.Y, bkgColor.Z, 0.0f);
    glPushMatrix();
    glColor3f(textColor.X, textColor.Y, textColor.Z);
    glRasterPos3f(5.0, 5.0, -5.0);

    debug_strings[0] = "======Keybinds======";
    debug_strings[1] = "Scream Internally and resist the urge to throw your computer out the window";
    debug_strings[2] = "Press 'q' to quit";
    debug_strings[3] = "Press 'w' to toggle wireframe";
    debug_strings[4] = "Press 's' to toggle solid";

    for (int i = 0; i < debug_strings->size(); i++) {
        // glRasterPos3f(5.0, lVportH - (i + 1) * 15, -5.0);
        glRasterPos3f(5.0, (i + 1) * 15, -5.0);
        std::string inString = std::string(debug_strings[i]);
        char *c;
        for (c = &inString[0]; *c != '\0'; c++) {
            glutBitmapCharacter((void *) GLUT_BITMAP_8_BY_13, *c);
            std::cout << *c;
        }
    }
    glPopMatrix();
}


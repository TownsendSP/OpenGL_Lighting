//
// Created by tgsp on 4/2/2024.
//
#include "globals.h"
#include "LeftVP.h"

#include <iostream>
#include <map>

#include "ColorData.h"

extern float rVPColor[];
extern ColorData rVPColorData;
extern ColorData solarizedBG;
extern ColorData solarizedText;
float scrollConsolePercent = 0.3;
int numConsoleLines;
extern std::map<int, std::string> debugMap;
extern std::vector<std::string> consoleMsgs;
extern int lVportW;
extern int height;



// void processConsoleMsgs() {
//     numConsoleLines = scrollConsolePercent * height / 15;
//     for(int i = 0; i < (consoleMsgs.size()<(numConsoleLines-1))?consoleMsgs.size():(numConsoleLines-1); i++) {
//         glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
//         glRasterPos3f(3.0, 0.3 * height - i * 15, -1.0);
//         std::for_each(consoleMsgs[i].msg.begin(), consoleMsgs[i].msg.end(),
//                       [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
//
//         consoleMsgs[i].time -= timeElapsedMillis;
//         if(consoleMsgs[i].time <= 0) {
//             consoleMsgs.erase(consoleMsgs.begin() + i);
//         }
//     }
// }


void processConsoleMsgs() {
    // Calculate the maximum height for messages
    int maxHeight = numConsoleLines * 15;

    // Iterate over the messages in reverse order (from newest to oldest)
    for (int i = consoleMsgs.size() - 1; i >= 0; --i) {
        // Calculate the height of the current message
        int messageHeight = (consoleMsgs.size() - i) * 15;

        // If the message's height is above the maximum, remove it
        if (messageHeight > maxHeight) {
            consoleMsgs.erase(consoleMsgs.begin() + i);
        }
        // Otherwise, draw the message
        else {
            glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
            glRasterPos3f(3.0, messageHeight, -1.0);
            std::for_each(consoleMsgs[i].msg.begin(), consoleMsgs[i].msg.end(),
                          [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
        }
    }
}

void drawLeft() {
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    //draw background with bkgColor; must be a rectangle, not glclearcolor
    glColor4f(solarizedBG.R, solarizedBG.G, solarizedBG.B, solarizedBG.A);
    // vertex array for the background
    GLint vertices[] = {
        0, 0,
        lVportW, 0,
        lVportW, height,
        0, height
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    //draw thick solarized orange line along the right edge
    glLineWidth(5.0f);
    glColor4f(0.86275, 0.19608, 0.18431, 1.0);
    glBegin(GL_LINES);
    glVertex2i(lVportW, 0);
    glVertex2i(lVportW, height);
    glEnd();

    //draw thick horizontal solarized orange line 20% from the bottom
    glLineWidth(2.50f);
    glBegin(GL_LINES);
    glVertex2i(0, scrollConsolePercent * height);
    glVertex2i(lVportW, scrollConsolePercent * height);
    glEnd();
    glLineWidth(1.0f);

    glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
    for (const auto &pair: debugMap) {
        glRasterPos3f(3.0, (pair.first + 1) * 15, -1.0);
        std::for_each(pair.second.begin(), pair.second.end(),
                      [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
    }
    processConsoleMsgs();
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void setupLeft() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glViewport(0, 0, lVportW, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, lVportW, 0, height, -10, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //add message to
}


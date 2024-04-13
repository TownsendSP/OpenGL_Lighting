//
// Created by tgsp on 4/9/2024.
//

#include "Scenedraw.h"
#include "Coord.h"
#include "ColorData.h"
#include "globals.h"
#include "things.h"
#include "lighting.h"
#include "camera.h"
extern std::map<int, std::string> debugMap;
extern Camera cam;

Coord hallBnl = Coord(0, 0, -2);
Coord halltfr = Coord(10, 3, 2);

//Coord roomBnl = Coord(halltfr.X, 0, 3*hallBnl.Z);
Coord roomBnl = Coord(halltfr.X, hallBnl.Y, 3 * hallBnl.Z);
Coord roomtfr = Coord(2 * halltfr.X, halltfr.Y, 3 * halltfr.Z);

Coord troubleshootingBnl = halltfr * 0.45;
Coord troubleshootingtfr = halltfr * 0.55;
Coord lampPos = Coord(5, 2.7, 0);

float t() {
    return pow(1.15, (-1.3 * (cam.pos.dist(lampPos) - 12))) + 1;
}

#include <vector>
#include <cmath>

std::vector<float> returnEvenlySpaced(int requestedPoints, float minDist, float maxVal) {
    if (requestedPoints <= 0 || minDist <= 0 || maxVal < 0) {
        return {};
    }
    float idealSpacing = (maxVal - minDist) / (requestedPoints - 1);
    if (idealSpacing >= minDist) {
        std::vector<float> points(requestedPoints);
        for (int i = 0; i < requestedPoints; ++i) {
            points[i] = minDist * i + minDist;
        }
        return points;
    } else {
        // Minimum distance cannot be satisfied, calculate how many points fit
        int actualPoints = std::floor((maxVal - minDist) / minDist) + 1;

        // Create the vector with adjusted spacing (maxVal divided by actual points)
        std::vector<float> points(actualPoints);
        float step = maxVal / (actualPoints - 1);
        for (int i = 0; i < actualPoints; ++i) {
            points[i] = i * step;
        }
        return points;
    }
}


Coord tableBnl = Coord(roomtfr.X - 2, roomBnl.Y, -1.5);
Coord tableTfr = Coord(roomtfr.X - 1, roomtfr.Y / 3, 1.5);
float legInset = 0.2;
float tableThickness = 0.1;
float legRadT = 0.1;
float legRadB = 0.1;


void makeLeg(float *legInfo) {
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    GLUquadricObj *quadric = gluNewQuadric();
    gluCylinder(quadric, legInfo[2], legInfo[0], legInfo[1], 32, 32);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void drawLeg(Coord legLocBottom, float legInfo[3]) {
    glPushMatrix();
    glTranslatefv(legLocBottom + Coord(0, legInfo[1], 0));
    castIronMat.apply();
    // make vertical cylinders
    makeLeg(legInfo);
    glPopMatrix();
}

void drawTable(Coord bnl, Coord tfr, float lInset = 0.2, float tThick = 0.1, float legRad = 0.1) {
    //table top
    tableMat.apply();
    cubeOfPlanes(Coord(bnl.X, tfr.Y - tThick, bnl.Z), tfr, 40, 1, ALL_FACE);


    /*     legs, use glutcylinders
     *     leg order:
     *      1--2
     *      |  |b
     *      0--3    */


    Coord leg0Loc = bnl + Coord(legInset, 0, legInset);
    Coord leg1Loc = Coord(tfr.X - legInset, 0, bnl.Z + legInset);
    // Coord leg1Loc = bnl + Coord(tfr.X-bnl.X - legInset, 0, legInset);
    Coord leg2Loc = tfr- Coord(legInset, tfr.Y, legInset);
    Coord leg3Loc = Coord(bnl.X +legInset, 0, tfr.Z - legInset);
    Coord legInfo = Coord(legRadT, tfr.Y - tThick - bnl.Y, legRadB);

    drawLeg(leg0Loc, legInfo);
    drawLeg(leg1Loc, legInfo);
    drawLeg(leg2Loc, legInfo);
    drawLeg(leg3Loc, legInfo);

    tfr.debug(0, 1, 0.75);
    bnl.debug(1, 1, 0.75);
    leg0Loc.debug(2);
leg1Loc.debug(3);
leg2Loc.debug(4);
leg3Loc.debug(5);

}


void drawLampCage() {
    //enable smooth lines
    glEnable(GL_LINE_SMOOTH);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glutWireSphere(0.32, 8, 5);
    glPopMatrix();
    glDisable(GL_LINE_SMOOTH);
}

void drawLamp() {
    lampMat.apply();
    glPushMatrix();
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    castIronMat.apply();
    glLineWidth(t());

    glPushMatrix();
    drawLampCage();
    glPopMatrix();
    glLineWidth(1);
}

void drawHall() {
    hallLight.setup();
    glEnable(GL_LIGHTING);
    hallLight.enable();

    glPushMatrix();
    glTranslatefv(lampPos);
    drawLamp();
    glPopMatrix();


    wallMat.apply();
    cubeOfPlanes(roomBnl, Coord(roomtfr.X, roomtfr.Y, hallBnl.Z), 40, -1, FRONT_FACE | BACK_FACE);
    cubeOfPlanes(Coord(halltfr.X, roomBnl.Y, halltfr.Z), roomtfr, 40, -1, FRONT_FACE | BACK_FACE);
    cubeOfPlanes(roomBnl, roomtfr, 40, -1, LEFT_FACE | RIGHT_FACE);


    floorMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, -1, BOTTOM_FACE);
    ceilingMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, -1, TOP_FACE);

    //this one does the gap in the back of the room, and the wall at 00
    wallMat.apply();
    cubeOfPlanes(hallBnl, Coord(roomtfr.X, halltfr.Y, halltfr.Z), 40, -1, FRONT_FACE | BACK_FACE);


    cubeOfPlanes(hallBnl, halltfr, 40, -1, LEFT_FACE | RIGHT_FACE);
    floorMat.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, -1, BOTTOM_FACE);
    ceilingMat.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, -1, TOP_FACE);


    //small box for testing, use the shiny textures
    cardMat.apply();
    cubeOfPlanes(troubleshootingBnl, troubleshootingtfr, 10, 1, ALL_FACE);


    //drawing the table
    drawTable(tableBnl, tableTfr);

    hallLight.disable();
}

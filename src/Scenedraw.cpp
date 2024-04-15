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
Coord halltfr = Coord(10, 4, 2);

//Coord roomBnl = Coord(halltfr.X, 0, 3*hallBnl.Z);
Coord roomBnl = Coord(halltfr.X, hallBnl.Y, 3 * hallBnl.Z);
Coord roomtfr = Coord(2 * halltfr.X, halltfr.Y, 3 * halltfr.Z);

Coord troubleshootingBnl = halltfr * 0.45;
Coord troubleshootingtfr = halltfr * 0.55;
Coord lampPos = Coord(roomBnl.X+5, halltfr.Y-0.3, 0);

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

float doorwayHeight = 3;
float doorwayWidth = 2;



void drawCardWinner1(int winnernum = 0) {
    // Set the color to yellow

    cardMat.apply();
    glColor3f(0, 1.0f, 1.0f);
    // Draw the flattened dodecahedron
    glPushMatrix();
    glScalef(1.0f, 1.0f, 0.01f); // Scale in Y dimension to flatten
    glutSolidDodecahedron();
    glPopMatrix();

    // Set the normal in the Y dimension
    // glNormal3f(0.0f, 1.0f, 0.0f);


    lampMat.apply();
    // Draw stroke text
    glLineWidth(5);
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-1.3f, -0.01f, 0.1f); // Position the text
    glScalef(0.0045f, 0.0045f, 0.0045f); // Scale the text

    std::string texts = retWinner();
    const char *text = texts.c_str();

    debugMap[60] = "             winner:" + std::string(texts);
    debugMap[61] = "             winner:" + std::string(text);
    while (*text) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *text++);
    }
    glPopMatrix();
}



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
    Coord legInfo = Coord(legRadT/4, tfr.Y - tThick - bnl.Y, legRadB/2);

    drawLeg(leg0Loc, legInfo);
    drawLeg(leg1Loc, legInfo);
    drawLeg(leg2Loc, legInfo);
    drawLeg(leg3Loc, legInfo);
}

void drawContainer(Coord bnl, Coord tfr) {
    //simple cube, no top
    glPushMatrix();
    shinyRed.apply();
    cubeOfPlanes(bnl, tfr, 20, 1, ALL_FACE ^ TOP_FACE);
    glPopMatrix();
}

 void lDoorExist() {
    //door: simple cube
    superShinySteel.apply();
    float animPercent = -2.0f*(doorOpenPercent / 100.0);
    glPushMatrix();
    glTranslatef(0, 0, animPercent);
    cubeOfPlanes(Coord(halltfr.X - 0.4, hallBnl.Y, hallBnl.Z),
                 Coord(halltfr.X - 0.1, hallBnl.Y + doorwayHeight, 0), 20, 1, ALL_FACE);
    glPopMatrix();
}

void rDoorExist() {
    //just reflect it over the Z, the normals will be "good enough"
    superShinySteel.apply();

glPushMatrix();
    glScalef(1, 1, -1);
    lDoorExist();
    glPopMatrix();

    //draw container


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

void lockInCardNormals() {
    glPushMatrix();
    glNormal3f(0, 0, 1);
    drawCardWinner1();
    glPopMatrix();
}

void drawCardFinalPos() {
    glPushMatrix();
    glRotatef(270, 0, 1, 0);
    glRotatef(270, 1, 0, 0);
    lockInCardNormals();
    glPopMatrix();
}

void drawWinnerAndRotate() {
    Coord finalRot = Coord(3*360, 7*360, 90);
    float dumb = cardRotPercent;
    float cardRot = dumb/(cardRotSpeed*100);
    Coord currentRot = finalRot * cardRot;

    glPushMatrix();
    glScalef(0.9f*cardRot+0.1f, 0.9f*cardRot+0.1f, 0.9f*cardRot+0.1f);
    glTranslatef(0, 3*cardRot, 0);
    glRotatef(currentRot.X, 1, 0, 0);
    glRotatef(currentRot.Y, 0, 1, 0);
    glRotatef(currentRot.Z, 0, 0, 1);
    drawCardFinalPos();
    glPopMatrix();
}

void drawHall() {
    hallLight.setup();
    glEnable(GL_LIGHTING);
    hallLight.disable();
    headLamp.enable();

    //get rid of this later:
    // glPushMatrix();
    // glTranslatefv(lampPos);
    // drawLamp();
    // glPopMatrix();
    // hallLight.enable();

    wallMat.apply();
    //doorframe, but only the back face:
    cubeOfPlanes(Coord(halltfr.X-0.5, hallBnl.Y, hallBnl.Z),
        Coord(halltfr.X, hallBnl.Y + doorwayHeight, hallBnl.Z + doorwayWidth/2), 20, 1, BACK_FACE);

    //top of doorframe
    cubeOfPlanes(Coord(halltfr.X-0.5, hallBnl.Y + doorwayHeight, hallBnl.Z),
        Coord(halltfr.X, halltfr.Y, halltfr.Z), 20, 1, BACK_FACE);

    //Left side of doorframe
    cubeOfPlanes(Coord(halltfr.X-0.5, hallBnl.Y, halltfr.Z - doorwayWidth/2),
        Coord(halltfr.X, hallBnl.Y + doorwayHeight, halltfr.Z), 20, 1, BACK_FACE);



    cubeOfPlanes(hallBnl, Coord(roomtfr.X, halltfr.Y, halltfr.Z), 40, 1, BACK_FACE);
    cubeOfPlanes(hallBnl, halltfr, 40, 1, LEFT_FACE | RIGHT_FACE);
    floorMat.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, 1, BOTTOM_FACE);
    ceilingMat.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, 1, TOP_FACE);


#ifndef FOLDING_REGION_ROOM


    glPushMatrix();
    glTranslatefv(lampPos);
    drawLamp();
    glPopMatrix();
    hallLight.enable();

    wallMat.apply();
    cubeOfPlanes(roomBnl, Coord(roomtfr.X, roomtfr.Y, hallBnl.Z), 40, 1, FRONT_FACE | BACK_FACE);
    matteConcrete.apply();
    cubeOfPlanes(Coord(halltfr.X, roomBnl.Y, halltfr.Z), roomtfr, 40, 1, FRONT_FACE | BACK_FACE);

    wallMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, 1, LEFT_FACE | RIGHT_FACE);
    floorMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, 1, BOTTOM_FACE);
    ceilingMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, 1, TOP_FACE);


    //doorframe, but not the back face:
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y, hallBnl.Z),
                 Coord(halltfr.X, hallBnl.Y + doorwayHeight, hallBnl.Z + doorwayWidth / 2),
                 20, 1,ALL_FACE ^ BACK_FACE);

    //top of doorframe
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y + doorwayHeight, hallBnl.Z),
                 Coord(halltfr.X, halltfr.Y, halltfr.Z), 20, 1, ALL_FACE ^ BACK_FACE);

    //Left side of doorframe
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y, halltfr.Z - doorwayWidth / 2),
                 Coord(halltfr.X, hallBnl.Y + doorwayHeight, halltfr.Z), 20, 1, ALL_FACE ^ BACK_FACE);


    //this one does the gap in the back of the room,
    wallMat.apply();
    cubeOfPlanes(hallBnl, Coord(roomtfr.X, halltfr.Y, halltfr.Z), 40, 1, FRONT_FACE);


    //small box for testing, use the shiny textures
    cardMat.apply();
    cubeOfPlanes(troubleshootingBnl, troubleshootingtfr, 10, 1, ALL_FACE);

    //doors:
    lDoorExist();
    rDoorExist();

    //drawing the table
    drawTable(tableBnl, tableTfr);
#endif

    glPushMatrix();
    glTranslatef(0.1 + tableBnl.X, tableTfr.Y, -0.25 );
    drawContainer(Coord(0,0,0), Coord(0.5,0.51,0.5));

    glPopMatrix();

    // glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(0.2 + tableBnl.X, tableTfr.Y, 0 );
    glScalef(0.25, 0.25, 0.25);
    drawWinnerAndRotate();
    glPopMatrix();



    glPushMatrix();
    glTranslatef(10, 10, 10);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, 1, FRONT_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, 1, TOP_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, 1, RIGHT_FACE);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, 1, BACK_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, 1, BOTTOM_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, 1, LEFT_FACE);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(7, 10, 10);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, -1, FRONT_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, -1, TOP_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, -1, RIGHT_FACE);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, -1, BACK_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, -1, BOTTOM_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord()+2.0f, 100, -1, LEFT_FACE);

    glPopMatrix();
            // glEnable(GL_LIGHTING);


    hallLight.disable();
}

//
// Created by tgsp on 4/3/2024.
//

#include "globals.h"
#include "things.h"
#include "Coord.h"
#include <map>

#include <iostream>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif


extern std::map<int, std::string> debugMap;


//Window Blinds Routines
#ifndef FOLDING_REGION_BLINDS

Blinds::Blinds(float width, float height, float depth, float pitchAngle, float closedFactor) {
    matSpecBlinds = ColorData(0.5, 0.5, 0.5, 1.0);
    matShineBlinds[0] = 50.0f;
    matAmbAndDifBlinds = ColorData(0.9, 0.9, 0.89, 1.0);

    this->width = width;
    this->height = height;
    this->depth = depth;
    this->pitchAngle = pitchAngle;
    this->numBlades = height / depth; // NOLINT(*-narrowing-conversions)
    this->closedFactor = 0.0;
}


void Blinds::drawBlade() const {
    //scale, rotate about Z, translate
    glPushMatrix();
    glRotatef(pitchAngle, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(depth, bladeHeight, width);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
}

void Blinds::drawDbgPoints(DebugLevel dbg) const {
    std::vector<Debug3Dx> points;
    Coord topLeftFront = Coord(-depth, height, width);
    points.emplace_back(&topLeftFront, 0.5, 1);
    Coord bottomRightBack = Coord(depth, -height, -width);
    points.emplace_back(&bottomRightBack, 0.5, 1);

    //if Strong or all, draw a point for each of the blades's centers


    // if debug == all, draw the bounding box for the bounding box for the blind apparatus
    if (dbg == ALL) {
        glDisable(GL_LIGHTING);
        glColor3f(0.9, 0.9, 0.9);
        glEnable(GL_LINE_STIPPLE);
        glLineWidth(2);

        glPushMatrix();
        glTranslatef(0.0, height / 2, 0.0);
        glScalef(depth, height, width);
        glutWireCube(1.0);
        glPopMatrix();

        glLineWidth(1);
        glDisable(GL_LINE_STIPPLE);
        glEnable(GL_LIGHTING);
    }

    glPushMatrix();
    glDisable(GL_LIGHTING);
    for (auto &point: points) {
        point.draw();
    }
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void Blinds::draw(DebugLevel dbg) const {
    // Set material properties.
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShineBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matAmbAndDifBlinds);

    float bladeSpacing = depth * closedFactor + bladeHeight / 2;
    float topBladeHeight = height - bladeSpacing;
    glPushMatrix();
    for (int i = 0; i < numBlades; i++) {
        // bool isBladeOpened = closedFactor * numBlades < i; // NOLINT(*-narrowing-conversions)
        glPushMatrix();
        glTranslatef(0.0, topBladeHeight - ((i - 1) * bladeSpacing), 0.0);
        drawBlade();
        glPopMatrix();
    }
    glPopMatrix();

    drawDbgPoints(dbg);

    if (debug_string_add_map_ != nullptr) {
        (*debug_string_add_map_)[60-17] = "Blinds State: " + std::to_string(closedFactor);
        (*debug_string_add_map_)[60-18] = "Blade Spacing: " + std::to_string(bladeSpacing);
        (*debug_string_add_map_)[60-19] = "Num Blades: " + std::to_string(numBlades);
    }
}

void drawBlinds(Blinds blindIn) {
    // Set material properties.
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blindIn.matSpecBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, blindIn.matShineBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blindIn.matAmbAndDifBlinds);
}

void Blinds::open(float amt) {
    closedFactor = closedFactor - amt < 0.0 ? 0.0 : closedFactor - amt;
}

void Blinds::close(float amt) {
    closedFactor = closedFactor + amt > 1.0 ? 1.0 : closedFactor + amt;
}

void Blinds::animate(float amt) {
    float current = closedFactor;
    if (opening) {
        current -= amt;
        opening = current > 0.0;
        closedFactor = current >= 0.0 ? current : 0.0;
    } else {
        current += amt;
        opening = current < 1.0;
        closedFactor = current <= 1.0 ? current : 1.0;
    }
}
#endif


//Drawing a 3D Debug Axis
#ifndef FOLDING_REGION_DEBUG3DX

Debug3Dx::Debug3Dx(Coord *position, float size, float weight) {
    this->size = size;
    this->weight = weight;
    this->position = *position;
}

Debug3Dx::Debug3Dx(Coord position, float size, float weight) {
    this->position = position;
    this->size = size;
    this->weight = weight;
}

Debug3Dx::Debug3Dx(float inStuff[5]) {
    this->position = {inStuff[0], inStuff[1], inStuff[2]};
    this->size = inStuff[3];
    this->weight = inStuff[4];
}

void Debug3Dx::draw() const {
    //set line width to weight
    glLineWidth(weight);
    glPushMatrix();

    // X axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X - size, position.Y, position.Z);
    glVertex3f(position.X + size, position.Y, position.Z);
    glEnd();

    //y axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X, position.Y - size, position.Z);
    glVertex3f(position.X, position.Y + size, position.Z);
    glEnd();

    //z axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X, position.Y, position.Z - size);
    glVertex3f(position.X, position.Y, position.Z + size);
    glEnd();

    glPopMatrix();


    glLineWidth(1);
}

void Debug3Dx::draw(Coord pos, float length, float weight) const {
    //create a new Debug3Dx object and draw it, then delete it
    Debug3Dx(pos, length, weight).draw();
    //delete it when done
}

void drawXZxGridlines(float range) {
    glColor3f(0.7f, 0.7f, 0.7f); // Set color to light gray
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0404);
    for (int i = -range; i <= range; ++i) { // NOLINT(*-narrowing-conversions)
        glBegin(GL_LINES);
        // x-axis
        glVertex3f(-range, 0.0f, static_cast<float>(i));
        glVertex3f(range, 0.0f, static_cast<float>(i));
        // z-axis
        glVertex3f(static_cast<float>(i), 0.0f, -range);
        glVertex3f(static_cast<float>(i), 0.0f, range);
        glEnd();
    }

    glDisable(GL_LINE_STIPPLE);
}

#endif

void drawBMPStr(std::string str, void *font) {
    for (int8_t c: str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void drawFlatPlane(Coord corner1, Coord corner2, int numSubDivisions) {
    // corner1 = corner1 - Coord(1, 1, 1);
    // corner2 = corner2 - Coord(1, 1, 1);
    // Calculate the size of each subdivision
    float xStep = (corner2.X - corner1.X) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float zStep = (corner2.Z - corner1.Z) / numSubDivisions;

    // Draw the grid of triangle strips
    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X + i * xStep, corner1.Y, corner1.Z + j * zStep);
            glVertex3f(corner1.X + (i + 1) * xStep, corner1.Y, corner1.Z + j * zStep);
        }
        glEnd();
    }
}

void drawPlane(Coord c1, Coord c2, Coord normalVec, int numSubDivisions) {
    //draw the flat plane with the correct dimensions and location

    //figure out how the final plane will need to be rotated based on the corners;


    // XZ plane: CORRECT!
    if (c1.Y - c2.Y <= 0.001) {
        glPushMatrix();
        glNormal3fv(normalVec);
        glTranslatefv(c1);
        drawFlatPlane(Coord(), c2 - c1, numSubDivisions);
        glPopMatrix();
    }

    // XY plane:x
    if (abs(c1.Z - c2.Z) <= 0.001) {
        glNormal3fv(normalVec);
        glPushMatrix();
        glTranslatefv(c1);
        glRotatef(90, -1.0, 0.0, 0.0);
        drawFlatPlane(Coord(), Coord((c2 - c1).X, 0, (c2 - c1).Y), numSubDivisions);
        glPopMatrix();
    }

    //YZ plane: rotate 90 degrees about the y axis
    if (abs(c1.X - c2.X) <= 0.001) {
        glNormal3fv(normalVec);
        glPushMatrix();
        glTranslatefv(c1);
        glRotatef(90, 0.0, 0.0, 1.0);
        drawFlatPlane(Coord(), Coord((c2-c1).Y, 0, (c2-c1).Z), numSubDivisions);
        glPopMatrix();
    }
}

void cubeGLfrom2Points(Coord bnl, Coord tfr, uint8_t mode) {
    //draw the cube with the correct dimensions and location
    glPushMatrix();
    glTranslatefv((tfr-bnl)/2+bnl);
    glScalefv(tfr-bnl);
if(mode == WiREFRAME)
    glutWireCube(1.0);
else if(mode == SOLID)
    glutSolidCube(1.0);
    glPopMatrix();
}

void cubeOfPlanes(Coord bnl, Coord tfr, int numSubDiv, int insideOut, uint8_t whichFaces) {
    /*draw the cube of planes with the correct dimensions and location
    whichFaces is a bitfield that determines which faces to draw; 1 is draw, 0 is don't draw
    1: front (YZ - X+), 2: top (XZ - Y+), 3: right (XY - Z+)
    4: back (YZ - X-), 5: bottom (XZ - Y-), 6: left (XY - Z-)*/
    //flat shading

    glPushMatrix();
    if (whichFaces >> 5 & 1) //far +X
    	drawPlane(Coord(tfr.X, bnl.Y, bnl.Z), tfr, Coord(-insideOut, 0, 0), numSubDiv);
    if (whichFaces >> 4 & 1) //top +Y
        drawPlane(Coord(bnl.X, tfr.Y, bnl.Z), tfr, Coord(0, insideOut, 0), numSubDiv);
    if (whichFaces >> 3 & 1) //right +Z
    	drawPlane(Coord(bnl.X, bnl.Y, tfr.Z), tfr, Coord(0, 0, -insideOut), numSubDiv);
    if (whichFaces >> 2 & 1) //near -X
    	drawPlane(bnl, Coord(bnl.X, tfr.Y, tfr.Z), Coord(-insideOut, 0, 0), numSubDiv);
    if (whichFaces >> 1 & 1) //bottom -Y
    	drawPlane(bnl, Coord(tfr.X, bnl.Y, tfr.Z), Coord(0, -insideOut, 0), numSubDiv);
    if (whichFaces & 1) //left -Z
    	drawPlane(bnl, Coord(tfr.X, tfr.Y, bnl.Z), Coord(0, 0, -insideOut), numSubDiv);

    glPopMatrix();
}

//
// Created by tgsp on 4/3/2024.
//

#include "things.h"

#include <iostream>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif
#define PI 3.14159




//Window Blinds Routines
Blinds::Blinds(float width, float height, float depth, float pitchAngle, float closedFactor) {
    matSpecBlinds = ColorData(0.5, 0.5, 0.5, 1.0);
    matShineBlinds[0] = 50.0f;
    matAmbAndDifBlinds = ColorData(0.8, 0.8, 0.7, 1.0);

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
    glScalef(depth, bladeHeight, width);
    glRotatef(pitchAngle, 0.0, 0.0, 1.0);
    glutSolidCube(1.0);
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

    float bladeSpacing = depth * closedFactor + bladeHeight;
    float topBladeHeight = height - bladeSpacing;
    glPushMatrix();
    for (int i = 0; i <= numBlades; i++) {
        // bool isBladeOpened = closedFactor * numBlades < i; // NOLINT(*-narrowing-conversions)
        glPushMatrix();
        glTranslatef(0.0, topBladeHeight-((i-1)*bladeSpacing), 0.0);
        drawBlade();
        glPopMatrix();
    }
    glPopMatrix();

    drawDbgPoints(dbg);
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

//Drawing a 3D Debug Axis
Debug3Dx::Debug3Dx(Coord *position, float size, float weight) {
    this->size = size;
    this->weight = weight;
    this->position = position;
}

// Debug3Dx::Debug3Dx(Coord position, float size, float weight) {
//     this->position = position;
//     this->size = size;
//     this->weight = weight;
// }

void Debug3Dx::draw() const {
    //set line width to weight
    glLineWidth(weight);
    glPushMatrix();

    // X axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position->X - size, position->Y, position->Z);
    glVertex3f(position->X + size, position->Y, position->Z);
    glEnd();

    //y axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(position->X, position->Y - size, position->Z);
    glVertex3f(position->X, position->Y + size, position->Z);
    glEnd();

    //z axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position->X, position->Y, position->Z - size);
    glVertex3f(position->X, position->Y, position->Z + size);
    glEnd();

    glPopMatrix();


    glLineWidth(1);
}

void drawXZxGridlines(float range) {
    glColor3f(0.7f, 0.7f, 0.7f); // Set color to light gray
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0404);
    for (int i = -range; i <= range; ++i) {
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
//
//
// void drawXZxGridlines(float range) {
//     glColor3f(0.7f, 0.7f, 0.7f); // Set color to light gray
//     glEnable(GL_LINE_STIPPLE);
//     glLineStipple(1, 0x0404);
//
//     float step = 1.0f; // Initial step size
//     for (float i = -range; i <= range; i += step) {
//         if (static_cast<int>(i) % 100 == 0 && i != 0) {
//             step *= 2; // Double the step size every 100 units
//         }
//
//         glBegin(GL_LINES);
//         // x-axis
//         glVertex3f(-range, 0.0f, i);
//         glVertex3f(range, 0.0f, i);
//         // z-axis
//         glVertex3f(i, 0.0f, -range);
//         glVertex3f(i, 0.0f, range);
//         glEnd();
//     }
//
//     glDisable(GL_LINE_STIPPLE);
// }


void drawBMPStr(std::string str, void* font) {
    for (int8_t c: str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}
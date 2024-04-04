//
// Created by tgsp on 4/3/2024.
//

#include "things.h"




//Window Blinds Routines
Blinds::Blinds(float width, float height, float depth, float pitchAngle, float closedFactor) {
    matSpecBlinds = ColorA(0.0, 1.0, 1.0, 1.0);
    matShineBlinds[0] = 50.0f;
    matAmbAndDifBlinds = ColorA(0.8, 0.8, 0.7, 1.0);

    this->width = width;
    this->height = height;
    this->depth = depth;
    this->pitchAngle = pitchAngle;
    this->numBlades = height/depth; // NOLINT(*-narrowing-conversions)
    this->closedFactor = 0.0;
}

void Blinds::drawBlade() const {//scale, rotate about Z, translate
    glPushMatrix();
    glScalef(depth, height, width);
    glRotatef(pitchAngle, 0.0, 0.0, 1.0);
    glutSolidCube(7.0);
    glPopMatrix();
}

void Blinds::draw() const {
    // Set material properties.
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShineBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matAmbAndDifBlinds);

    float bladeSpacing = depth * closedFactor + bladeHeight;

    for (int i = 0; i >= numBlades; i++) {
        bool isBladeOpened = closedFactor*numBlades < i;  // NOLINT(*-narrowing-conversions)
        glPushMatrix();
        glTranslatef(0.0, isBladeOpened? bladeSpacing : bladeHeight, 0.0);
        drawBlade();
        glPopMatrix();
    }
    glPopMatrix();

}

void Blinds::open(float amt) {
    closedFactor = closedFactor- amt < 0.0 ? 0.0 : closedFactor - amt;
}

void Blinds::close(float amt) {
    closedFactor = closedFactor + amt > 1.0 ? 1.0 : closedFactor + amt;
}

void Blinds::animate(float amt) {
    float current = closedFactor;
    if(opening) {
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
// Debug3Dx::Debug3Dx(float size, float weight, Coord position) {
//     this -> size = size;
//     this -> weight = weight;
//     this -> position = &position;
// }

Debug3Dx::Debug3Dx(Coord *position, float size, float weight) {
    this -> size = size;
    this -> weight = weight;
    this -> position = position;
}

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



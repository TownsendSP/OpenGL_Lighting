//
// Created by tgsp on 4/4/2024.
//

#include "lighting.h"




void Light::setup() {
    glLightfv(whichLight, GL_AMBIENT, lightAmb);
    glLightfv(whichLight, GL_DIFFUSE, lightDiff);
    glLightfv(whichLight, GL_SPECULAR, lightSpec);
    glLightfv(whichLight, GL_POSITION, lightPos);
}

void Spotlight::setup() {
    Light::setup();
    glLightfv(whichLight, GL_SPOT_DIRECTION, spotDir);
    glLightf(whichLight, GL_SPOT_CUTOFF, spotAngle);
    glLightf(whichLight, GL_SPOT_EXPONENT, spotAttenuation);
}


void Spotlight::drawVis() {
    // // Rotate to match the direction of the spotlight
    // // Assuming spotDir is a direction vector, we need to convert it to an angle and an axis
    // float angle = acos(spotDir[2]); // Angle with the z-axis
    //
    //
    // float coneHeight = 12.0; // Height of the cone
    // //calc cone radius based on cutoff angle
    // float coneRadius = coneHeight * tan(spotAngle * (PI / 180.0)); // Radius of the cone
    //
    //
    //
    // glPushMatrix(); // Save matrix state
    //
    // // Translate to the position of the spotlight
    // glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
    //
    //
    // glRotatef(angle * (180.0 / PI), -spotDir[1] / axisLength, 0, spotDir[0] / axisLength); // Rotate around the cross product of the direction and the z-axis
    //
    // // Set the color and transparency of the material
    // glColor4f(lightDiff[0], lightDiff[1], lightDiff[2], 0.2); // 20% opaque
    //
    // // Draw the cone
    // // Assuming the base radius and height of the cone are 1. Adjust these values as needed.
    // // Also assuming we have a suitable level of detail for the cone. Adjust these values as needed.
    // glutSolidCone(1, 1, 20, 20);
    //
    // glPopMatrix(); // Restore matrix state
}


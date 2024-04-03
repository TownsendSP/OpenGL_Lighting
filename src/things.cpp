//
// Created by tgsp on 4/3/2024.
//

#include "things.h"

Blinds::Blinds(float width, float height, float depth, float pitchAngle) {
    matSpecBlinds = ColorA(0.0, 1.0, 1.0, 1.0);
    matShineBlinds[0] = 50.0f;
    matAmbAndDifBlinds = ColorA(0.8, 0.8, 0.7, 1.0);

    this->width = width;
    this->height = height;
    this->depth = depth;
    this->pitchAngle = pitchAngle;

}






#include "Camera.hpp"

//#include "bee.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#define PI 3.14159

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

Camera::Camera(float position[3], float target[3], float orientation[3], float trans_speed, float rot_sens) {

    pos = Coord(position[0], position[1], position[2]);
    tgt = Coord(target[0], target[1], target[2]);
    up = Coord(orientation[0], orientation[1], orientation[2]);
    ang = calcPitchYaw(pos, tgt); //pitch, yaw, roll

    transSpeed = trans_speed;
    rotSens = rot_sens;
}

Camera::Camera(Coord Pos, Coord Target, Coord orientation, float trans_speed, float rot_sens) {
    pos = Pos;
    tgt = Target;
    up = orientation;
    ang = calcPitchYaw(pos, tgt); //pitch, yaw, roll
}

Camera::Camera(Coord Pos, Coord Target, Coord orientation) {
    pos = Pos;
    tgt = Target;
    up = orientation;
    ang = calcPitchYaw(pos, tgt); //pitch, yaw, roll
}

void Camera::setPitchYaw() {
    Coord dirVec = (tgt - pos);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    ang = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
}


int Camera::useDebugStringAdd(int line, std::string message) {
    if (debug_string_add_) {
        return debug_string_add_(line, message);
    } else {
        std::cerr << "debug_string_add_ function is not set." << std::endl;
        return -1;
    }
}

Coord calcPitchYaw(Coord position, Coord target) {
    Coord dirVec = (target - position);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    Coord rv = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
    return rv;
}

Coord calcTarget(Coord position, Coord pitchYaw) {
    float dirX = cos(pitchYaw.X) * cos(pitchYaw.Y);
    float dirY = sin(pitchYaw.Y);
    float dirZ = sin(pitchYaw.X) * cos(pitchYaw.Y);
    return position + Coord(dirX, dirY, dirZ);
}

// void Camera::relTrans(Coord deltaTranslation) {
//     //calculate the current direction vector
//     Coord transVec = Coord(cos(ang.Y) * cos(ang.X), sin(ang.X), sin(ang.Y) * cos(ang.X)) * deltaTranslation;
//     pos = pos + transVec;
//     tgt = tgt + transVec;
// }

// void Camera::relTrans(Coord deltaTranslation) {
//     float cosPitch = cos(ang.X);
//     float sinPitch = sin(ang.X);
//     float cosYaw = cos(ang.Y);
//     float sinYaw = sin(ang.Y);
//
//     Coord transVec;
//     transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
//     transVec.Y = deltaTranslation.X * sinPitch * sinYaw + deltaTranslation.Y * cosPitch + deltaTranslation.Z * sinPitch * cosYaw;
//     transVec.Z = deltaTranslation.X * cosPitch * sinYaw - deltaTranslation.Y * sinPitch + deltaTranslation.Z * cosPitch * cosYaw;
//
//
//     pos = pos + transVec;
//     tgt = tgt + transVec;
// }


void Camera::relTrans(Coord deltaTranslation) {

    float cosYaw = cos(ang.Y);
    float sinYaw = sin(ang.Y);


    Coord transVec;
    transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
    transVec.Y = deltaTranslation.Y;
    transVec.Z = deltaTranslation.X * sinYaw + deltaTranslation.Z * cosYaw;


    pos = pos + transVec;
    tgt = tgt + transVec;
}

void Camera::relRot(Coord deltaAngle) {
    Coord tAng = ang + deltaAngle;
    float a = PI / 2.0f;
    float b = tAng.X;
    float c = std::min(a, b);
    float d = -PI / 2.0f;
    tAng.X = std::max(d, c);
    // tAng.X = std::max(-PI / 2.0f, std::min((static_cast<float>PI)/ 2.0f, tAng.X));

    tAng.Y = fmod(tAng.Y, 2 * PI);
    if (tAng.Y < 0) {
        tAng.Y += 2 * PI;
    }

    ang = tAng;
    Coord dirVec = Coord(cos(tAng.Y) * cos(tAng.X), sin(tAng.X), sin(tAng.Y) * cos(tAng.X));
    tgt = pos + dirVec;
}

void Camera::lookAt(DebugLevel dbg) {
    gluLookAt(pos.X, pos.Y, pos.Z, tgt.X, tgt.Y, tgt.Z, up.X, up.Y, up.Z);
    if(dbg != NONE) {
        std::vector<std::string> debugToAdd = toString();
        for (int i = 0; i < debugToAdd.size(); i++) {
            useDebugStringAdd(i+30, debugToAdd[i]);
        }
    }
}

std::vector<std::string> Camera::toString() const {
    std::vector<std::string> retVal;
#include <sstream>
    Coord dirVec = Coord(cos(ang.Y) * cos(ang.X), sin(ang.X), sin(ang.Y) * cos(ang.X));

    retVal.emplace_back(std::string("Position X: " + std::to_string(pos.X) + ", Y: " + std::to_string(pos.Y) + ", Z: " + std::to_string(pos.Z)));
    retVal.emplace_back(std::string("Target X: " + std::to_string(tgt.X) + ", Y: " + std::to_string(tgt.Y) + ", Z: " + std::to_string(tgt.Z)));
    retVal.emplace_back(std::string("Orientation X: " + std::to_string(up.X) + ", Y: " + std::to_string(up.Y) + ", Z: " + std::to_string(up.Z)));
    retVal.emplace_back(std::string("Agnle: Pitch " + std::to_string(ang.X) + ", Yaw " + std::to_string(ang.Y) + ", Roll " + std::to_string(ang.Z)));
    retVal.emplace_back(std::string("Direction Vector: X " + std::to_string(dirVec.X) + ", Y " + std::to_string(dirVec.Y) + ", Z " + std::to_string(dirVec.Z)));
    return retVal;
}
// void Camera::relRot(float pitchChange, float yawChange) {
//
//
//     float tpitch += pitchChange;
//     float tyaw += yawChange;
//
//     // clamping the pitch, preventing backflips
     // pitch = std::max(-PI / 2.0f, std::min(PI / 2.0f, pitch));
//
//
//     float dirX = cos(yaw) * cos(pitch);
//     float dirY = sin(pitch);
//     float dirZ = sin(yaw) * cos(pitch);
//
//     // applying the calculated directions
//     centerx = eyeX + dirX;
//     centery = eyeY + dirY;
//     centerz = eyeZ + dirZ;
// }


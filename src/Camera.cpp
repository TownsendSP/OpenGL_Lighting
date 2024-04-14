

#include "Camera.h"
#include "globals.h"

//#include "bee.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
#include <streambuf>
#include <iostream>
#include <string>


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

Coord Camera::normDirVec() {
    return (tgt - pos) / sqrt((tgt - pos).pow(2).sum());
}

int Camera::saveToFile(const std::string& filename) {
    // Open the file for output using the filename
    std::ofstream file(filename);

    if (!file.is_open()) {
        glout << "Error: File is not open" << std::endl;
        std::cerr << "Error: File is not open" << std::endl;
        return -1;
    }
    for (std::tuple<Coord, Coord> state: storedStates) {
        file << std::get<0>(state).toHexString() << "," << std::get<1>(state).toHexString() << std::endl;
    }

    if (file.fail()) {
        glout << "Error: Failed to save camera" << std::endl;
        std::cerr << "Error: Failed to save camera" << std::endl;
        return -2;
    }
    file.close();
    return 0;
}

std::vector<float> sToF(const std::string &input, char delimiter) {
    std::vector<float> floats;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        // Convert hex string to float
        double d = std::strtod(token.c_str(), nullptr);
        floats.push_back(static_cast<float>(d));
    }

    return floats;
}

std::vector<std::string> readFileLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return lines;
    }
    std::string lineForWriting;
    while (std::getline(file, lineForWriting)) {
        lines.push_back(lineForWriting);
    }

    file.close();
    return lines;
}

std::vector<float> splitAndConvertToFloats(const std::string &input, char delimiter) {
    std::vector<float> floats;
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        // Convert hex string to float
        double d = std::strtod(token.c_str(), nullptr);
        floats.push_back(static_cast<float>(d));
    }

    return floats;
}

int Camera::loadFromFile(const std::string& filename) {
    std::vector<std::string> lines = readFileLines(filename);
    std::vector<float> floats;
    int i = 0;
    for (const auto& lineForIteration : lines) {
        floats = splitAndConvertToFloats(lineForIteration, ',');
        storedStates[i] = std::make_tuple(Coord(floats[0], floats[1], floats[2]), Coord(floats[3], floats[4], floats[5]));
        // glout << "Loaded state " << i << " as " << std::get<0>(storedStates[i]).toString() << ", " << std::get<1>(storedStates[i]).toString() << std::endl;
        i++;
    }
    return 0;
}

//int Camera::loadFromFile(const std::string& filename) {
//    // Open the file using the filename
//    std::ifstream file(filename);
//
//    if (!file.is_open()) {
//        std::cout << "Error: File is not open" << std::endl;
//        return -1;
//    }
//
//    std::string line;
//    int i = 0;
//
//    // for each line in the file, split on commas; the first
//    std::vector<float> f;
//    // load line 0 into a string and print it:
//    std::cout << "Loading camera from file: " << filename << std::endl;
//    std::getline(file, line);
//
//
//
//
//
//
//
//    std::cout << "First line: " << line << std::endl;
//    while (i < 5 && std::getline(file, line)) {
//        f = sToF(line, ',');
//        storedStates[i] = std::make_tuple(
//            Coord(f[0], f[1], f[2]),
//            Coord(f[3], f[4], f[5]));
//        std::cout << "i: " << line << " loaded as \n\t" << f[0] << ", " << f[1] << ", " << f[2] << ", " << f[3] << ", "
//                << f[4] << ", " << f[5] << std::endl;
//        i++;
//    }
//
//    if (file.fail() && !file.eof()) {
//        std::cerr << "Error: Failed to load camera" << std::endl;
//        return -2;
//    }
//    file.close();
//    return 0;
//}

void Camera::setPitchYaw() {
    Coord dirVec = (tgt - pos);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    ang = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
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
    //begin Pitch clamping to ensure no backflips or frontflips
    float a = PI / 2.0f;
    float b = tAng.X;
    float c = std::min(a, b);
    float d = -PI / 2.0f;
    tAng.X = std::max(d, c);
    //end Pitch clamping

    tAng.Y = fmod(tAng.Y, 2 * PI);
    if (tAng.Y < 0) {
        tAng.Y += 2 * PI;
    }

    ang = tAng;
    Coord dirVec = Coord(cos(tAng.Y) * cos(tAng.X), sin(tAng.X), sin(tAng.Y) * cos(tAng.X));
    tgt = pos + dirVec;
    dirVecPublicUsage = dirVec;
}

void Camera::lookAt(DebugLevel dbg) {
    gluLookAt(pos.X, pos.Y, pos.Z, tgt.X, tgt.Y, tgt.Z, up.X, up.Y, up.Z);
        std::vector<std::string> debugToAdd = toString(2);
        for (unsigned int i = 0; i < debugToAdd.size(); i++) {
            (*debug_string_add_map_)[i + 45] = debugToAdd[i];
        }
}

void Camera::useDebugStringAdd(int lineInt, std::string message) {
    (*debug_string_add_map_)[lineInt] = message;
}

std::vector<std::string> Camera::toString(int prec) const {
    std::vector<std::string> retVal;
#include <sstream>
    std::string* p = pos.toStrings(prec);
    std::string* t = tgt.toStrings(prec);
    std::string* u = up.toStrings(prec);
    std::string* a = ang.toStrings(prec);
    std::string* d = Coord(cos(ang.Y) * cos(ang.X), sin(ang.X), sin(ang.Y) * cos(ang.X)).toStrings(prec);

    retVal.emplace_back(std::string("DirVec: X " + d[0] + ", Y " + d[1] + ", Z " + d[2]));
    retVal.emplace_back(std::string("Agnle: Pitch " + a[0] + ", Yaw " + a[1] + ", Roll " + a[2]));
    retVal.emplace_back(std::string("Orientation X: " + u[0] + ", Y: " + u[1] + ", Z: " + u[2]));
    retVal.emplace_back(std::string("Target X: " + t[0] + ", Y: " + t[1] + ", Z: " + t[2]));
    retVal.emplace_back(std::string("Pos X: " + p[0] + ", Y: " + p[1] + ", Z: " + p[2]));
    retVal.emplace_back("          Camera Info");
    return retVal;
}

std::string floatToHexString(float f) {
    std::stringstream stream;
    stream << std::hex << f;
    return stream.str();
}

float hexStringToFloat(std::string s) {
    float f;
    std::stringstream stream;
    stream << s;
    stream >> std::hex >> f;
    return f;
}

#include <cmath>

#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif

#include "Coord.hpp"

#include <cmath>

#define DEBUG

#include <algorithm>
#include <iostream>
#include <random>
#include <iomanip>
#include <sstream>


std::string trunc(float fl, int prec) {
    char buffer[50];
    std::snprintf(buffer, sizeof(buffer), "%.*f", prec, fl);
    return buffer;
}

float dist(Coord point1, Coord point2) {
    float distX = std::abs(point1.X - point2.X);
    float distY = std::abs(point1.Y - point2.Y);
    float distZ = std::abs(point1.Z - point2.Z);

    return std::sqrt(distX * distX + distY * distY + distZ * distZ);
}

float srnd(float min, float max) {
    if (min > max) {
        std::swap(min, max);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

float srndi(float min, float max) {
    return static_cast<int>(srnd(min, max + 1));
}


float randomNaiveNotSeeded(float min, float max) {
    float randthing = min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
// #ifdef DEBUG
//   std::cout << "randthing: " << randthing << std::endl;
// #endif

    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
}

std::vector<Coord> genRandNonCoLinearCord(int numCoords, float permissibleCloseness, Coord min, Coord max) {
    std::vector<Coord> coords;
    for (int i = 0; i < numCoords; i++) {
        Coord tempCoord;
        tempCoord = randCoord(min, max);
        bool isCoLinear = false;
        for (auto &coord: coords) {
            if (tempCoord.dist(coord) < permissibleCloseness) {
                isCoLinear = true;
                break;
            }
        }
        if (!isCoLinear) {
            coords.push_back(tempCoord);
        }
    }
    return coords;
}

std::string* Coord::toStrings(int precision) const{
    std::string* ret = new std::string[3];
    ret[0] = trunc(X, precision);
    ret[1] = trunc(Y, precision);
    ret[2] = trunc(Z, precision);
    return ret;
}

std::string* Coord::toStrings(int precision) {
    std::string* ret = new std::string[3];
    ret[0] = trunc(X, precision);
    ret[1] = trunc(Y, precision);
    ret[2] = trunc(Z, precision);
    return ret;
}

std::string Coord::toString(int precision) {
    std::string* things = toStrings(precision);
    std::string str = "(" + things[0] + ", " + things[1] + ", " + things[2] + ")";
    return str;
}


Coord genRandNonCoLinearCord(std::vector<Coord> coordsIn, float permissibleCloseness, Coord min, Coord max) {
    std::vector<Coord> coords = coordsIn;;
    for (int i = 0; i < 1; i++) {
        Coord tempCoord;
        tempCoord = randCoord(min, max);
        bool isCoLinear = false;
        for (auto &coord: coords) {
            if (tempCoord.dist(coord) < permissibleCloseness) {
                isCoLinear = true;
                break;
            }
        }
        if (!isCoLinear) {
            return tempCoord;
        }
    }
    return randCoord(min, max);

}


bool isColorCloseEnough(Coord color1, Coord color2, float closeness) {
    if (abs(color1.X - color2.X) < closeness &&
        abs(color1.Y - color2.Y) < closeness &&
        abs(color1.Z - color2.Z) < closeness) {
        return true;
        }
    return false;
}




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

std::string Coord::toString() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << std::showpoint;
    oss << "(" << X << ", " << Y << ", " << Z << ")";
    std::string str = oss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
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

bool isColorCloseEnough(ColorA color1, ColorA color2, float closeness) {
    if (abs(color1.R - color2.R) < closeness &&
        abs(color1.G - color2.G) < closeness &&
        abs(color1.B - color2.B) < closeness) {
        return true;
    }
    return false;
}


bool isCo
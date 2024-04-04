#include <cmath>

#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif

#ifndef COORD_H
#define COORD_H

#include <algorithm>

#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

float srnd(float min, float max);


inline float clmp(float val, float min, float max) {
    return std::fmax(min, std::fmin(val, max));
}

//simple function to scale a value from one range to another
inline float clscl(float val, float min, float max) {
    return (val - min) / (max - min);
}

float srndi(float min, float max);


class Coord {
public:
    float X, Y, Z;

    //constructor
    Coord(float x, float y, float z) {
        X = x;
        Y = y;
        Z = z;
    }

    explicit Coord(const float in[3]) {
        X = in[0];
        Y = in[1];
        Z = in[2];
    }

    //default:
    Coord() {
        X = 0;
        Y = 0;
        Z = 0;
    }


    //mathy stuff
    Coord operator+(const Coord &other) const {
        return {X + other.X, Y + other.Y, Z + other.Z};
    }

    Coord operator+(const float other) const {
        return {X + other, Y + other, Z + other};
    }

    [[nodiscard]] float sum() const {
        return X + Y + Z;
    }

    [[nodiscard]] Coord clamp(float min, float max) const {
        return {clmp(X, min, max),
                clmp(Y, min, max),
                clmp(Z, min, max)};
    }

    [[nodiscard]] Coord clamp(Coord min, Coord max) const {
        return {clmp(X, min.X, max.X),
                clmp(Y, min.Y, max.Y),
                clmp(Z, min.Z, max.Z)};
    }

    [[nodiscard]] Coord scale(float min, float max) const {
        return {clscl(X, min, max),
                clscl(Y, min, max),
                clscl(Z, min, max)};
    }

    [[nodiscard]] Coord scale(Coord min, Coord max) const {
        return {clscl(X, min.X, max.X),
                clscl(Y, min.Y, max.Y),
                clscl(Z, min.Z, max.Z)};
    }

    Coord operator-(const Coord &other) const {
        return {X - other.X, Y - other.Y, Z - other.Z};
    }

    Coord operator-(const float other) const {
        return {X - other, Y - other, Z - other};
    }

    Coord operator*(const Coord &other) const {
        return {X * other.X, Y * other.Y, Z * other.Z};
    }

    Coord operator*(const float other) const {
        return {X * other, Y * other, Z * other};
    }

    Coord operator/(const Coord &other) const {
        if (other.X != 0 && other.Y != 0 && other.Z != 0) {
            return {X / other.X, Y / other.Y, Z / other.Z};
        } else {
            return {};
        }
    }

    Coord operator/(const float other) const {
        if (other != 0) {
            return {X / other, Y / other, Z / other};
        } else {
            return {};
        }
    }

    Coord operator&(const int i) const {
        return Coord(i & 4 ? X : 0, i & 2 ? Y : 0, i & 1 ? Z : 0); // 4 = 100, 2 = 010, 1 = 001
    }

    Coord operator|(const Coord i) const {
        return {X == 0 ? i.X : X, Y == 0 ? i.Y : Y, Z == 0 ? i.Z : Z};
    }

    Coord operator|(const float i) const {
        return {X == 0 ? i : X, Y == 0 ? i : Y, Z == 0 ? i : Z};
    }

    bool operator==(const Coord &other) const {
        return X == other.X && Y == other.Y && Z == other.Z;
    }

    [[nodiscard]] Coord pow(const float exponent) const {
        return {std::pow(X, exponent), std::pow(Y, exponent), std::pow(Z, exponent)};
    }

    [[nodiscard]] Coord pow(const Coord &other) const {
        return {std::pow(X, other.X), std::pow(Y, other.Y), std::pow(Z, other.Z)};
    }

    [[nodiscard]] float dist(const Coord &other) const {
        return std::sqrt(
                std::abs(X - other.X) * std::abs(X - other.X) +
                std::abs(Y - other.Y) * std::abs(Y - other.Y) +
                std::abs(Z - other.Z) * std::abs(Z - other.Z));
    }

    // std::string toString () const {
    //     return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ")";
    // }


// std::string toString () const {
//     std::ostringstream oss;
//     oss << std::fixed << std::setprecision(2) << std::showpoint;
//     oss << "(" << X << ", " << Y << ", " << Z << ")";
//     std::string str = oss.str();
//     str.erase(str.find_last_not_of('0') + 1, std::string::npos);
//     str.erase(str.find_last_not_of('.') + 1, std::string::npos);
//     return str;
// }

    std::string toString();
};

inline Coord randCoord(const Coord &min, const Coord &max) {
    return {srnd(std::min(min.X, max.X), std::max(min.X, max.X)),
            srnd(std::min(min.Y, max.Y), std::max(min.Y, max.Y)),
            srnd(std::min(min.Z, max.Z), std::max(min.Z, max.Z))};
}

float dist(Coord point1, Coord point2);

std::vector<Coord> genRandNonCoLinearCord(int numCoords, float permissibleCloseness, Coord min, Coord max);

Coord genRandNonCoLinearCord(std::vector<Coord> coords, float permissibleCloseness, Coord min, Coord max);

#endif //COORD_H

bool isColorCloseEnough(Coord color1, Coord color2, float closeness);



class ColorA {
public:
    float R, G, B, A;

    ColorA(float r, float g, float b, float a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    explicit ColorA(const float in[4]) {
        R = in[0];
        G = in[1];
        B = in[2];
        A = in[3];
    }

    //default:
    ColorA() {
        R = 0;
        G = 0;
        B = 0;
        A = 0;
    }

    //<editor-fold desc="Arithmetic">
    ColorA operator+(const ColorA &other) const {
        return {R + other.R, G + other.G, B + other.B, A + other.A};
    }

    ColorA operator+(const float other) const {
        return {R + other, G + other, B + other, A + other};
    }

    ColorA operator-(const ColorA &other) const {
        return {R - other.R, G - other.G, B - other.B, A - other.A};
    }

    ColorA operator-(const float other) const {
        return {R - other, G - other, B - other, A - other};
    }

    ColorA operator*(const ColorA &other) const {
        return {R * other.R, G * other.G, B * other.B, A * other.A};
    }

    ColorA operator*(const float other) const {
        return {R * other, G * other, B * other, A * other};
    }

    ColorA operator/(const ColorA &other) const {
        if (other.R != 0 && other.G != 0 && other.B != 0 && other.A != 0) {
            return {R / other.R, G / other.G, B / other.B, A / other.A};
        } else {
            return {};
        }
    }

    ColorA operator/(const float other) const {
        if (other != 0) {
            return {R / other, G / other, B / other, A / other};
        } else {
            return {};
        }
    }
    //</editor-fold>


    //<editor-fold desc="Bitwise">
    ColorA operator&(const int i) const {
        return ColorA(i & 8 ? R : 0, i & 4 ? G : 0, i & 2 ? B : 0,
                      i & 1 ? A : 0); // 8 = 1000, 4 = 0100, 2 = 0010, 1 = 0001
    }

    ColorA operator|(const ColorA i) const {
        return {R == 0 ? i.R : R, G == 0 ? i.G : G, B == 0 ? i.B : B, A == 0 ? i.A : A};
    }

    ColorA operator|(const float i) const {
        return {R == 0 ? i : R, G == 0 ? i : G, B == 0 ? i : B, A == 0 ? i : A};
    }
    //</editor-fold>

    //otherMath:
    ColorA clamp(float min, float max) const {
        return {clmp(R, min, max),
                clmp(G, min, max),
                clmp(B, min, max),
                clmp(A, min, max)};
    }

    ColorA clamp(ColorA min, ColorA max) const {
        return {clmp(R, min.R, max.R),
                clmp(G, min.G, max.G),
                clmp(B, min.B, max.B),
                clmp(A, min.A, max.A)};
    }

    ColorA scale(float min, float max) const {
        return {clscl(R, min, max),
                clscl(G, min, max),
                clscl(B, min, max),
                clscl(A, min, max)};
    }

    ColorA scale(ColorA min, ColorA max) const {
        return {clscl(R, min.R, max.R),
                clscl(G, min.G, max.G),
                clscl(B, min.B, max.B),
                clscl(A, min.A, max.A)};
    }

    ColorA pow(const float exponent) const {
        return {std::pow(R, exponent), std::pow(G, exponent), std::pow(B, exponent), A};
    }

    ColorA pow(const ColorA &other) const {
        return {std::pow(R, other.R), std::pow(G, other.G), std::pow(B, other.B), A};
    }

    float dist(const ColorA &other) const {
        return std::sqrt(
                std::abs(R - other.R) * std::abs(R - other.R) +
                std::abs(G - other.G) * std::abs(G - other.G) +
                std::abs(B - other.B) * std::abs(B - other.B));
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << std::showpoint;
        oss << "(" << R << ", " << G << ", " << B << ", " << A << ")";
        std::string str = oss.str();
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        return str;
    }


    bool operator==(const ColorA &other) const {
        return R == other.R && G == other.G && B == other.B && A == other.A;
    }

    ColorA scale255() const {
        return {R * 255, G * 255, B * 255, A};
    }

    ColorA scale1() const {
        return {R / 255, G / 255, B / 255, A};
    }

    Coord toCoord() const {
        return {R, G, B};
    }

    //toArray
    float *toArray() {
        return new float[4]{R, G, B, A};
    }

    operator float*() const {
    return new float[4]{R, G, B, A};
}
operator Coord () const {
    return {R, G, B};
}
};


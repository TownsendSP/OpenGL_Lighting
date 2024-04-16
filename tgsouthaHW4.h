/****************************************************************************************
* Official Name:   Townsend Southard Pantano
* E-Mail:          tgsoutha@syr.edu
* Assignment:      Homework 4
* Environment:     CLion EAP 2024.1.0.0
* Compiler:        Visual Studio 2022 Compiler
* Generator:       CMake 3.27
* Toolchain:       Visual Studio 2022
* Date:            4Apr2024
* References:      Computer Graphics Thropugh OpenGL 2nd Edition,
*                      Sumanta Guha, 2015
*                      ISBN: 978-1-4822-5840-0
*                   Slides from Lecture
*                   Nothing was copied and pasted, but those sources were used as examples
*					Microsoft Documentaiton:
*						https://learn.microsoft.com/en-us/cpp/standard-library/output-streams?view=msvc-170
*						https://stackoverflow.com/questions/78133486/c-input-output-stream
*						https://learn.microsoft.com/en-us/windows/win32/opengl/gl-functions
*
* 	i: camera forward
* 	j: camera backwards
* 	k: camera strafe left
* 	l: camera strafe right
* 	;: camera +Y (semicolon)
* 	.: camera -Y (period)
* 	d: activate door and room light
* 	alt+d: activate door
* 	shift+d: activate light
* 	Space: Toggle mouse-Controlled camera
* 	Shift+Space: Disable Collision
* 	1-5: Load saved camera position and rotation
* 	Shift+1-5: Save Cameera position and rotation
* 	Alt+1-6: Disable cube faces. In order, by their normals: +X, +Y, +Z, -X, -Y, -Z
* 	`: (backtick) Rotate through colorings of the custom normals. In order:
* 		Absolute Value, Normalized to 1, Positive Normals only, Negative Normals only, Default lit view
* 	Alt+`: Re-enable all the cube faces
* 	G: increase global ambient light
* 	g: decrease global ambient light
* 	h: toggle headlamp
* 	w: activate window animations
* 	?: Print keybinds to the informational console
*
* 	F1: Shows/hides the information viewport
* 	F2: Shrinks the console
* 	F3: Grows the console
* 	F4: Exact same thing as 'h'
* 	F5: Loads camera positions from "cam.txt"
* 	F6: Opens the door, exactly as 'd' or alt+d function
* 	F7: Randomizes the winner
* 	Shift+F7: 	Sets the winner to the computer's username using unix commands, with a fallback to "User"
* 	Alt+F7: 	Sets the winner to the day of the week
* 	F8: Toggles between animating the card out of and back into its box
* 	Alt+F8: sets the card to use the camera distance as the factor for animation progression
* 	F9: Save the 5 available camera positions and rotations to disk
* 	F12: Debugging tool for printing the entire character sequence available in bitmap characters
* 	F11: Shows the clickable volumes for color picking, starts a countdown to hid the volumes after a certain amount of time.
* 		The animation timer is used for the countdown
* 	Alt+F11: Toggles "Select Locking" mode. While "Select Locking" is enabled, the hidden buffer will be visible as long as the right click button is held down. While the right click button is held down, even if a clickable volume was under the mouse during the click, the clickable volume under the mouse when the left button is released will be selected
* 	Right Arrow: Rotates the camera clockwise about the Y axis relative to the origin, since there is no roll
* 	Left Arrow: Rotates the camera counterclockwise about the Y axis
* 	Page Up: Rotates the camera clockwise about the Z axis relative to the camera.
* 	Page Down: Rotates the camera counterclockwise about the Z axis relative to the camera.
* 	HOME/END: Increase/Decrease the FOV, respectively
*
****************************************************************************************/

//                  __            __      __          __       __
//    ______________\ \    ____ _/ /___  / /_  ____ _/ /____  / /_
//   / ___/ ___/ ___/\ \  / __ `/ / __ \/ __ \/ __ `/ / ___/ / __ \
//  (__  ) /  / /__   \ \/ /_/ / / /_/ / /_/ / /_/ / (__  ) / / / /
// /____/_/   \___/    \_\__, /_/\____/_.___/\__,_/_/____(_)_/ /_/
//                      /____/

#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif
#include <chrono>
#include <cstdint>
#include <streambuf>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <map>
#include <map>
#include <string>
#include <vector>
#include <vector>

int_fast8_t xx8(char input);
int_fast16_t xx16(int input);
int_fast32_t xx32(long input);
int_fast64_t xx64(long input);
char xd8(int_fast8_t input);
short xd16(int_fast16_t input);
long xd32(int_fast32_t input);
long xd64(int_fast64_t input);
std::string xs8(int_fast8_t value);
std::string xs16(int_fast16_t value);
std::string xs32(int_fast32_t value);
std::string xs64(int_fast64_t value);

#ifndef FOLDING_REGION_GLOBALDEFS


#define PI 3.14159f
#define D(radians) ((radians) * 180.0 / PI)
#define R(degrees) ((degrees) * PI / 180.0)

#define WiREFRAME 0x19
#define SOLID 0x1A

//used for determining whether to convert color values to 0-1 range
#define PLZCONVERT true
#define NOCONVERT false

//cube faces, so I don't need to remember the order, or the bit stuff, I can do it just like GLUT defines stuff
#define ALL_FACE         0b111111
#define FRONT_FACE       0b100000
#define TOP_FACE         0b010000
#define RIGHT_FACE       0b001000
#define BACK_FACE        0b000100
#define BOTTOM_FACE      0b000010
#define LEFT_FACE        0b000001

#define XZ 0b101
#define YZ 0b011
#define XY 0b110


//defines for console:
#define CONTROLOFF xx8(0x00)
#define CONTROLON xx8(0xFF)
#define NOPREFIX xx8(0x03)
#define CLEARALL xx8(0x04)
#define PREFIX xx8(0x05)
#define LINEFEED xx8(0x06)
#define NEWLINE xx8(0x09)
#define DEL xx8(0x0A)
#define GROW xx8(0x7f)
#define SHRINK xx8(0x80)
#define MAX xx8(0x81)
#define DEFAULT xx8(0x82)

#define DOOR_OPENING 1
#define DOOR_CLOSING 0
#define DOOR_OPENED_STOPPED 2
#define DOOR_CLOSED_STOPPED 3

#define INSIDEOUT (-1)
#define OUTSIDEOUT 1

#define UNAMEWIN 1;
#define UNAMELOSE 0;

#define CARD_ROT_REL 4
#define CARD_ROT_COMPLETE 2
#define CARD_ROT_UNDO 3
#define CARD_ROT_NOW 1
#define CARD_ROT_NONE 0

#define DBG_NORM_OFF 0
#define ABS_NORM 1
#define RAW_NORM 2
#define POS_NORM 3
#define NEG_NORM 4


#define IN_ROOM 0
#define IN_HALL 1

#define COLL_DOOR 0x0001
#define COLL_FAR_YZ 0x0002
#define COLL_NEAR_YZ 0x0004
#define COLL_NONE_YZ 0x000F

#define COLL_HALL_LYX 0x001F
#define COLL_HALL_RYX 0x002F
#define COLL_ROOM_LYX 0x004F
#define COLL_ROOM_RYX 0x008F
#define COLL_NONE_YX 0x00F0

#define COLL_UP_XZ 0x01FF
#define COLL_DOWN_XZ 0x02FF
#define COLL_NONE_XZ 0x0F00

#define DBG_NORM_OFF 0
#define ABS_NORM 1
#define RAW_NORM 2
#define POS_NORM 3
#define NEG_NORM 4

#endif



enum DebugLevel {
    NONE,
    WEAK,
    STRONG,
    ALL
};

// its getting annoying not being able to pass in arrays to glTranslateStuff.
// this way I can pass in arrays or a Coord, since my Coords auto-cast
inline void glTranslatefv(float in[3]) {
    glTranslatef(in[0], in[1], in[2]);
};

inline void glScalefv(float in[3]) {
    glScalef(in[0], in[1], in[2]);
};

#ifndef FOLDING_REGION_CUSTOM_CONSOLE
extern float conHeightPercent;

class GLStreamOut : public std::streambuf {
private:
    bool pref = true;
    bool control = true;
    std::string buffer;
    std::vector<std::string> glConsoleVec;

protected:
    bool parseControlChars(char in_char);


    virtual int_type overflow(int_type c);

    std::streambuf::int_type sync();

public:
    const std::vector<std::string> &getConsole() const;

    //remove message n from console vector:
    void rm(int n);

    //make glConsole castable to std::vector<std::string>
    operator std::vector<std::string>() const { return glConsoleVec; }
    //return size of console vector
    int size() const { return glConsoleVec.size(); }
    //index operator
    std::string operator[](int i) const { return glConsoleVec[i]; }
};

extern GLStreamOut glConsole;
extern std::ostream glout;

class GLInfo : public std::streambuf {
private:
    std::string buffer;
    std::map<int, std::string> glInfoMap;

protected:
    virtual int_type overflow(int_type c);

    std::streambuf::int_type sync();

public:
    const std::map<int, std::string> &getInfo() const;
};

extern GLInfo glStatus;
extern std::ostream glInfoOut;
#endif


const std::string cameraSaveFile = "cam.txt";

#ifndef FOLDING_REGION_ATTEMPT_AT_ARBITRARY_DEBUG_POINTS
//this might still contribute to the memory leak
class thingHolder {
public:
    float a, b, c, d, e;

    explicit thingHolder(const float input[5]) {
        a = input[0];
        b = input[1];
        c = input[2];
        d = input[3];
        e = input[4];
    }

    thingHolder(float x, float x1, float x2, float size, float weight) {
        a = x;
        b = x1;
        c = x2;
        d = size;
        e = weight;
    }

    operator float *() {
        static float array[5] = {a, b, c, d, e};
        return array;
    }

    //default
    thingHolder() {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        e = 0;
    }
};

std::vector<thingHolder> getDbgPts();

extern std::vector<thingHolder> staticPoints;


void addDbgPt(int idx, float xyz[3], float size, float weight);

float *getDbgPts(int which);

int getNextPoint(int current);
#endif


extern int8_t doorOpenPercent;
extern int8_t animateDoor;

extern int winner;
int useTimeToSeedRandomToSetWinner();
std::string getDayOfWeek();
std::string getUName();
std::string retWinner();

extern int cardRotState;
extern int cardRotPercent;
extern int cardRotSpeed;
extern int dbgNormals;
extern uint8_t enabledFaces;
int nextDbgState();
extern std::map<int, std::string> dbgNormMap;

void glNormal3fvd(float whyAreMyNormalsBroken[3]);



extern bool selecting;
extern int xClick;
extern int yClick;
extern float globAmb[4];

extern float hallBnlF[3];
extern float halltfrF[3];
extern float roomBnlF[3];
extern float roomtfrF[3];
extern bool useCollision;
extern float moveSpeed;
extern float cardDist;

extern bool bufferPeeking;
extern bool windowFocused;
extern bool selectLock;
extern bool selectLockingEnabled;


extern int animatingBlinds;
extern int blindsProgress;

#endif //GLOBALS_H


//    ______                     __   __
//   / ____/___  ____  _________/ /  / /_
//  / /   / __ \/ __ \/ ___/ __  /  / __ \
// / /___/ /_/ / /_/ / /  / /_/ /_ / / / /
// \____/\____/\____/_/   \__,_/(_)_/ /_/

#ifndef COORD_H
#define COORD_H

//imports
// #include "globals.h"
#include <algorithm>
#include <cmath>
#include <functional>
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

    Coord(std::string hexString);

    std::string toHexString(char delim = ',');

    explicit Coord(const float* in) {
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
#ifndef FOLDING_COORDMATH
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

    // Coord operator*(const int lhs, const Coord & rhs) {
    //     return {rhs.X * lhs, lhs * rhs.Y, lhs * rhs.Z};
    // }

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
    //swapping will be done by overloading rightshift

    Coord operator>>(const uint8_t m) {
        return(
            (m==XZ) ? Coord(Z, Y, X) :
            (m==YZ) ? Coord(X, Z, Y) :
            (m==XY) ? Coord(X, Y, Z) : *this
            );
    };



#endif

    Coord wrap(Coord maxes, Coord mins, Coord add);

    Coord wrap(float maxe, float min, Coord add);

    operator float* () const {
        return new float[3]{X, Y, Z};
    }



    Coord vecToAngles() const {
        return {atan2(Z, X), asin(Y), 0};

    }

    Coord radiansToDegrees() const;

    Coord degToRad();

    Coord dirVecToRad() const;

    Coord radiansToDirVec() const;

    std::string *toStrings(int precision) const;

    std::string toString(int precision = 2);
    std::string* toStrings(int precision = 2);

    void debug(int idx, float s = 0.5, float w = 0.5);

};

inline Coord randCoord(const Coord &min, const Coord &max) {
    return {srnd(std::min(min.X, max.X), std::max(min.X, max.X)),
            srnd(std::min(min.Y, max.Y), std::max(min.Y, max.Y)),
            srnd(std::min(min.Z, max.Z), std::max(min.Z, max.Z))};
}

float dist(Coord point1, Coord point2);

std::vector<Coord> genRandNonCoLinearCord(int numCoords, float permissibleCloseness, Coord min, Coord max);

Coord genRandNonCoLinearCord(std::vector<Coord> coords, float permissibleCloseness, Coord min, Coord max);
bool isColorCloseEnough(Coord color1, Coord color2, float closeness);
#endif //COORD_H




#ifndef ALLCOMBINED_H
#define ALLCOMBINED_H



class allcombined {

};


//                  __      ______      __           ____        __          __
//    ______________\ \    / ____/___  / /___  _____/ __ \____ _/ /_____ _  / /_
//   / ___/ ___/ ___/\ \  / /   / __ \/ / __ \/ ___/ / / / __ `/ __/ __ `/ / __ \
//  (__  ) /  / /__   \ \/ /___/ /_/ / / /_/ / /  / /_/ / /_/ / /_/ /_/ / / / / /
// /____/_/   \___/    \_\____/\____/_/\____/_/  /_____/\__,_/\__/\__,_(_)_/ /_/

#ifndef COLORDATA_H
#define COLORDATA_H

// #include "globals.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
// #include "Coord.h"




class ColorData {
public:
    float R, G, B, A;
    float *X = &R;
    float *Y = &G;
    float *Z = &B;
    float *W = &A;


    ColorData(float r, float g, float b, float a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }

    // ColorData(int r, int g, int b, float a, uint8_t conv = NOCONVERT) {
    //         R = conv?(r/ 255.0):r;
    //         G = conv?(g/ 255.0):g;
    //         B = conv?(b/ 255.0):b;
    //         A = a;
    // }
    ColorData(uint32_t hexColor, float a) {
        R = ((hexColor >> 16) & 0xFF) / 255.0;
        G = ((hexColor >> 8) & 0xFF) / 255.0;
        B = (hexColor & 0xFF) / 255.0;
        A = a;
    }

    ColorData(const float in[4]) {
        R = in[0];
        G = in[1];
        B = in[2];
        A = in[3];
    }

    //default:
    ColorData() {
        R = 0.0f;
        G = 0.0f;
        B = 0.0f;
        A = 0.0f;
    }

    ColorData(bool a) {
        R = 0.0f;
        G = 0.0f;
        B = 0.0f;
        A = 0.0f;
    }

    ColorData(GLdouble r, GLdouble g, GLdouble b, GLdouble a) {
        R = r;
        G = g;
        B = b;
        A = a;
    }
    ColorData(Coord qq, float a) {
        R = qq.X;
        G = qq.Y;
        B = qq.Z;
        A = a;
    }

#ifndef ARITHMETIC_OPERATORS

    ColorData operator+(const ColorData &other) const {
        return {R + other.R, G + other.G, B + other.B, A + other.A};
    }

    ColorData operator+(const float other) const {
        return {R + other, G + other, B + other, A + other};
    }

    ColorData operator-(const ColorData &other) const {
        return {R - other.R, G - other.G, B - other.B, A - other.A};
    }

    ColorData operator-(const float other) const {
        return {R - other, G - other, B - other, A - other};
    }

    ColorData operator*(const ColorData &other) const {
        return {R * other.R, G * other.G, B * other.B, A * other.A};
    }

    ColorData operator*(const float other) const {
        return {R * other, G * other, B * other, A * other};
    }

    ColorData operator/(const ColorData &other) const {
        if (other.R != 0 && other.G != 0 && other.B != 0 && other.A != 0) {
            return {R / other.R, G / other.G, B / other.B, A / other.A};
        } else {
            return {};
        }
    }

    ColorData operator/(const float other) const {
        if (other != 0) {
            return {R / other, G / other, B / other, A / other};
        } else {
            return {};
        }
    }
#endif


    //<editor-fold desc="Bitwise">
    ColorData operator&(const int i) const {
        return ColorData(i & 8 ? R : 0, i & 4 ? G : 0, i & 2 ? B : 0,
                         i & 1 ? A : 0); // 8 = 1000, 4 = 0100, 2 = 0010, 1 = 0001
    }

    ColorData operator|(const ColorData i) const {
        return {R == 0 ? i.R : R, G == 0 ? i.G : G, B == 0 ? i.B : B, A == 0 ? i.A : A};
    }

    ColorData operator|(const float i) const {
        return {R == 0 ? i : R, G == 0 ? i : G, B == 0 ? i : B, A == 0 ? i : A};
    }

    //</editor-fold>

    //otherMath:
    ColorData clamp(float min, float max) const {
        return {
            clmp(R, min, max),
            clmp(G, min, max),
            clmp(B, min, max),
            clmp(A, min, max)
        };
    }

    ColorData clamp(ColorData min, ColorData max) const {
        return {
            clmp(R, min.R, max.R),
            clmp(G, min.G, max.G),
            clmp(B, min.B, max.B),
            clmp(A, min.A, max.A)
        };
    }

    ColorData scale(float min, float max) const {
        return {
            clscl(R, min, max),
            clscl(G, min, max),
            clscl(B, min, max),
            clscl(A, min, max)
        };
    }

    ColorData scale(ColorData min, ColorData max) const {
        return {
            clscl(R, min.R, max.R),
            clscl(G, min.G, max.G),
            clscl(B, min.B, max.B),
            clscl(A, min.A, max.A)
        };
    }

    ColorData pow(const float exponent) const {
        return {std::pow(R, exponent), std::pow(G, exponent), std::pow(B, exponent), A};
    }

    ColorData pow(const ColorData &other) const {
        return {std::pow(R, other.R), std::pow(G, other.G), std::pow(B, other.B), A};
    }

    float dist(const ColorData &other) const {
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


    bool operator==(const ColorData &other) const {
        return R == other.R && G == other.G && B == other.B && A == other.A;
    }

    ColorData scale255() const {
        return {R * 255, G * 255, B * 255, A};
    }

    ColorData scale1() const {
        return {R / 255, G / 255, B / 255, A};
    }

    Coord toCoord() const {
        return {R, G, B};
    }

    //toArray
    float *toArray() {
        return new float[4]{R, G, B, A};
    }

    GLfloat *toGLfloatArray() {
        return new GLfloat[4]{R, G, B, A};
    }

    // operator GLfloat*() const {
    //     return new GLfloat[4]{R, G, B, A};
    // }

    operator float *() const {
        return new float[4]{R, G, B, A};
    }


    operator Coord() const {
        return {R, G, B};
    }
};


bool isColorCloseEnough(ColorData color1, ColorData color2, float closeness);

class QuadPos : public ColorData {
};


#endif //COLORDATA_H

#endif //ALLCOMBINED_H

//                  __      ______
//    ______________\ \    / ____/___ _____ ___  ___  _________ _ _________  ____
//   / ___/ ___/ ___/\ \  / /   / __ `/ __ `__ \/ _ \/ ___/ __ `// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /___/ /_/ / / / / / /  __/ /  / /_/ // /__/ /_/ / /_/ /
// /____/_/   \___/    \_\____/\__,_/_/ /_/ /_/\___/_/   \__,_(_)___/ .___/ .___/
//                                                                 /_/   /_/

#ifndef CAMERA_H
#define CAMERA_H

// #include "globals.h"
#include <complex.h>
#include <map>
#include <vector>
// #include "Coord.h"


class Camera {

    std::string *debug_string_add_;
    std::map<int, std::string> *debug_string_add_map_;

public:
    std::tuple<Coord, Coord> storedStates[5];
    Coord pos;
    Coord tgt;
    Coord up;
    Coord ang;
    float transSpeed;
    float rotSens;
    Coord dirVecPublicUsage;
    int whichPlace;

    // constructor:

    Camera(float position[3], float target[3], float orientation[3], float trans_speed, float rot_sens);

    Camera(Coord pos, Coord target, Coord orientation, float trans_speed, float rot_sens);

    Camera(Coord Pos, Coord Target, Coord orientation);

    Coord normDirVec();

    int saveToFile(const std::string &filename);

    int loadFromFile(const std::string &filename);

    void storeState(int index) {
        storedStates[index] = std::make_tuple(pos, tgt);
    }

    void restoreState(int index) {

        pos = std::get<0>(storedStates[index]);
        tgt = std::get<1>(storedStates[index]);
        lookAt();
    }

    void setPitchYaw();


    Camera() = default;

    int whichSpace(Coord posi);

    int collisionYZ(Coord pos);

    int collisionYX(Coord pos);

    int collisionXZ(Coord pos);

    void getRelTrans(Coord deltaTranslation);

    void moveCamWithColl(Coord deltaTranslation);

    // calculating movement
    void relTrans(Coord deltaTranslation);

    void relRot(Coord deltaAngle);

    void updateCardInfo();

    void lookAt(DebugLevel dbg = NONE);

    std::vector<std::string> toString(int prec) const;

    // std::vector<std::string> toString() const;

    // void relRot(float pitch, float yaw);

    void setDebugStringAdd(std::map<int, std::string> *mapPtr) {
        this->debug_string_add_map_ = mapPtr;
    }

    void useDebugStringAdd(int lineInt, std::string message);

    Coord vec() const {
        return Coord(cos(ang.Y) * cos(ang.X), sin(ang.X), sin(ang.Y) * cos(ang.X));
    };
};

Coord calcPitchYaw(Coord position, Coord target);

Coord calcTarget(Coord position, Coord pitchYaw);

std::string trunc(float fl, int prec = 3);

void mapInsert(std::map<int, std::string> *mapthing, std::vector<std::string>, int indexInsert);
#endif //CAMERA_H


//                  __        __         ______ _    ______    __
//    ______________\ \      / /   ___  / __/ /| |  / / __ \  / /_
//   / ___/ ___/ ___/\ \    / /   / _ \/ /_/ __/ | / / /_/ / / __ \
//  (__  ) /  / /__   \ \  / /___/  __/ __/ /_ | |/ / ____/ / / / /
// /____/_/   \___/    \_\/_____/\___/_/  \__/ |___/_/   (_)_/ /_/

#ifndef TGSOUTHAHW4_LEFTVP_H
#define TGSOUTHAHW4_LEFTVP_H

// #include "globals.h"
#include <string>
// #include "Coord.h"


extern float conHeightPercent;
void drawLeft();
void setupLeft();

#endif //TGSOUTHAHW4_LEFTVP_H

//                  __        ___       __    __  _               __
//    ______________\ \      / (_)___ _/ /_  / /_(_)___  ____ _  / /_
//   / ___/ ___/ ___/\ \    / / / __ `/ __ \/ __/ / __ \/ __ `/ / __ \
//  (__  ) /  / /__   \ \  / / / /_/ / / / / /_/ / / / / /_/ / / / / /
// /____/_/   \___/    \_\/_/_/\__, /_/ /_/\__/_/_/ /_/\__, (_)_/ /_/
//                            /____/                  /____/

#ifndef LIGHTING_H
#define LIGHTING_H
// #include "globals.h"
// #include "ColorData.h"


class Light {
public:
    enum lightNum {
        LIGHT0 = GL_LIGHT0,
        LIGHT1 = GL_LIGHT1,
        LIGHT2 = GL_LIGHT2,
        LIGHT3 = GL_LIGHT3,
        LIGHT4 = GL_LIGHT4,
        LIGHT5 = GL_LIGHT5,
        LIGHT6 = GL_LIGHT6,
        LIGHT7 = GL_LIGHT7
    };

    bool enabled = true;

    ColorData lightAmb;
    ColorData lightDiff;
    ColorData lightSpec;
    ColorData lightPos;
    lightNum whichLight;


    Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diff, ColorData spec);;

    Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diffspec);;

    void setup();

    void enable() const;

    void disable() const;

    void lightswitch();;

    void lightswitch(bool onoff);;


    // define a cast so I can use the light in glEnable
    operator lightNum() const;

    //default constructor:
    Light();
};

class Spotlight : public Light {
public:
    Coord spotDir; //angle: X rot, Y rot, Z rot, 1.0
    float spotAttenuation;
    float spotAngle;

    Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, ColorData dir,
              float cutoff, float exponent);;
    Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, Coord dir,
              float cutoff, float exponent);;

    Spotlight();

    Spotlight(Light i, Coord dir, float cutoff, float exponent);

    void setup();

    void drawVis();
};




class Material {
public:
    Material() = default;

private:
    ColorData matSpec;
    ColorData matAmb;
    ColorData matEmission;
    ColorData matDiff;
    float matShine;

public:
    //constructor:

    Material(ColorData spec, ColorData amb, ColorData diff, ColorData emission, float shine);;

    Material(ColorData spec, ColorData amb, ColorData diff, float shine);

    //applyMaterial:
    void apply();

    //default constructor:
};

extern Material wallMat;
extern Material floorMat;
extern Material ceilingMat;
extern Material lampMat;
extern Material cardMat;
extern Material castIronMat;
extern Material tableMat;
extern Material superShinySteel;
extern Material matteConcrete;
extern Material shinyRed;
extern Material shinyGreen;
extern Material shinyBlue;


//light Library:
extern Light roomLight;
extern Light hallLight;
extern Light sunLight;
extern Spotlight headLamp;



#endif //LIGHTING_H

//                  __       _____                          __                      __
//    ______________\ \     / ___/________  ____  ___  ____/ /________ __      __  / /_
//   / ___/ ___/ ___/\ \    \__ \/ ___/ _ \/ __ \/ _ \/ __  / ___/ __ `/ | /| / / / __ \
//  (__  ) /  / /__   \ \  ___/ / /__/  __/ / / /  __/ /_/ / /  / /_/ /| |/ |/ / / / / /
// /____/_/   \___/    \_\/____/\___/\___/_/ /_/\___/\__,_/_/   \__,_/ |__/|__(_)_/ /_/

#ifndef SCENEDRAW_H
#define SCENEDRAW_H
// #include "lighting.h"

void drawHall();
void drawHiddenBuffer();
void drawClickableObjects();

#endif //SCENEDRAW_H

//                  __      __  __    _                    __
//    ______________\ \    / /_/ /_  (_)___  ____ ______  / /_
//   / ___/ ___/ ___/\ \  / __/ __ \/ / __ \/ __ `/ ___/ / __ \
//  (__  ) /  / /__   \ \/ /_/ / / / / / / / /_/ (__  ) / / / /
// /____/_/   \___/    \_\__/_/ /_/_/_/ /_/\__, /____(_)_/ /_/
//                                        /____/

#ifndef TGSOUTHAHW4_THINGS_H
#define TGSOUTHAHW4_THINGS_H

#include <map>

// #include "Camera.h"
// #include "ColorData.h"
// #include "Coord.h"
// #include "globals.h"
// #include "things.h"


// class things {
//
// };

void drawBMPStr(std::string str, void* font = GLUT_BITMAP_8_BY_13);

class Debug3Dx {
public:
    float size;
    float weight;
    Coord position;

    explicit Debug3Dx(Coord* position, float size = 1.0, float weight = 1.0);

    explicit Debug3Dx(Coord position, float size = 1.0, float weight = 1.0);

   Debug3Dx(float inStuff[5]);

    // Debug3Dx(float size = 1.0, float weight = 1.0, Coord position);

    void draw() const;
    void draw(Coord pos, float length = 0.5, float weight = 0.5) const;
};

void drawXZxGridlines(float range);



class Blinds{
public:
    float width;
    float height;
    float depth;
    float pitchAngle;
    std::map<int, std::string>* debug_string_add_map_ = nullptr;

    int numBlades;
    float bladeHeight = 0.01;

    float closedFactor;
    void drawBlade() const;

    void drawDbgPoints(DebugLevel dbg = NONE) const;

    bool opening = false;


    void setDebugStringAdd(std::map<int, std::string>* mapPtr) {
        this->debug_string_add_map_ = mapPtr;
    }
    float* matSpecBlinds;
    float matShineBlinds[1]{};
    float* matAmbAndDifBlinds;
    Blinds(float width, float height, float depth, float pitchAngle, float closedFactor = 0.0);
    void draw(DebugLevel dbg = NONE) const;

    std::vector<std::string> toStrings(int prec);

    void open(float amt);
    void close(float amt);
    void animate(float amt);

    Blinds() = default;
};


void drawPlane(Coord corner1, Coord corner2, Coord normalVec, int numSubDivisions);
void drawFlatPlaneXZ(Coord corner1, Coord corner2, int numSubDivisions);
void cubeOfPlanes(Coord bnl, Coord tfr, int numSubDiv, int insideOut = 1, uint8_t whichFaces = ALL_FACE);
void cubeGLfrom2Points(Coord bnl, Coord tfr, uint8_t mode = SOLID);


inline uint8_t animInfo = 0b000;
inline Coord coneRotAnim = Coord(0, 0, 0);
inline float coneHeight = 3;
inline float coneRotSpeed = 5;
extern std::map<int, std::string> debugMap;


void testConeArot(Coord angle = coneRotAnim);
void testConeA(float height, ColorData colorThing = ColorData(1.0f, 0.0f, 1.0f, 1.0f));
void testConeCrot(Coord angle, ColorData coloraaaa) ;
void testInRightPlace(Camera cam);

void testInRightPlace();
void testDrawingPlanes();
void testDrawingCubes();
void windowTest();

#endif //TGSOUTHAHW4_THINGS_H

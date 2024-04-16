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

//
// Created by tgsp on 4/15/2024.
//

#include "tgsouthaHW4.h"
//                  __            __      __          __
//    ______________\ \    ____ _/ /___  / /_  ____ _/ /____ _________  ____
//   / ___/ ___/ ___/\ \  / __ `/ / __ \/ __ \/ __ `/ / ___// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /_/ / / /_/ / /_/ / /_/ / (__  )/ /__/ /_/ / /_/ /
// /____/_/   \___/    \_\__, /_/\____/_.___/\__,_/_/____(_)___/ .___/ .___/
//                      /____/                                /_/   /_/

// # include "globals.h"

#include <chrono>
#include <iostream>
#include <map>
#include <random>


// #define NOPREFIX 0x03
// #define PREFIX 0x05
// #define CLEARALL 0x04
// #define LINEFEED 0x06
// #define NEWLINE 0x07
// #define DEL 0x08
// #define GROW
// float conHeightPercent = 0.3;


bool GLStreamOut::parseControlChars(char in_char) {
    //cast to hex for comparison
    // unsigned char c = in_char;
    // uint_fast16_t c = xx8(in_char);

    int_fast8_t c = xx8(in_char);
    if (c == xx8(0xFF) || control == true) {
        // std::cout << "Control char: " << c << std::endl;
        switch (c) {
            case 0x00:
                control = 0;
            // std::cout << "Control off" << std::endl;
                return false;
            case 0xFF:
                control = 1;
            // std::cout << "Control on" << std::endl;
                return false;
            case 0x03:
                pref = 0;
            // std::cout << "No prefix" << std::endl;
                return false;
                break;
            case 0x04: //clear buffer and console
                // std::cout << "Clear all" << std::endl;
                buffer.clear();
                glConsoleVec.clear();
                pref = true;
                glConsoleVec.push_back("$>");
                buffer.clear();

                return false;
            case 0x05:
                // std::cout << "Prefix On" << std::endl;
                pref = 1;
                return false;
            case 0x06:
                glConsoleVec.push_back(buffer);
            // std::cout << "Linefeed" << std::endl;
                buffer.clear();
                return false;
            case 0x09:
                glConsoleVec.push_back(buffer);
                buffer.clear();
                buffer += "  ";
            // std::cout << "Carriage return" << std::endl;
                return false;
            case 0x0A:
                if (buffer.size() > 2)
                    buffer.pop_back();
                else if (glConsoleVec.size() > 1)
                    glConsoleVec.pop_back();
            // std::cout << "Delete" << std::endl;
                return false;
            case 0x7f:
                conHeightPercent += 0.1;
                if (conHeightPercent > 0.52)
                    conHeightPercent = 0.52;
            // std::cout << "Grow" << std::endl;
                return false;
            case 0x80:
                conHeightPercent -= 0.1;
                if (conHeightPercent < 0.0)
                    conHeightPercent = 0.0;
                return false;
            case 0x81:
                conHeightPercent = 0.52;
            // std::cout << "MAX" << std::endl;
                return false;
            case 0x82:
                conHeightPercent = 0.3;
            // std::cout << "Default" << std::endl;
                return false;
            default:
                return true;
        }
    } else {
        return true;
    }
}

std::streambuf::int_type GLStreamOut::overflow(std::streambuf::int_type c) {
    if (c != EOF) {
        // the buffer should always start with $> to look like a cli
        if (buffer.empty() && pref) {
            buffer += "$>";
        }
        // Append the character to the buffer
        buffer += c;
        if (c == '\n') {
            glConsoleVec.push_back(buffer);
            buffer.clear();
        }
    }
    return c;
}


std::streambuf::int_type GLStreamOut::sync() {
    std::string parsedBuffer;
    for (char c: buffer) {
        // std::streambuf::int_type parsedChar = parseControlChars(c);
        if (parseControlChars(reinterpret_cast<std::streambuf::int_type &>(c)))
            parsedBuffer += c;
        // if (parsedChar != EOF) {
        //     parsedBuffer += parsedChar;
        // }
    } //std::cout << "after parsing: " << parsedBuffer << "before: " << buffer << std::endl;
    if (!parsedBuffer.empty()) {
        //add the shell prompt to parsedBuffer
        glConsoleVec.push_back(">$" + parsedBuffer);
        parsedBuffer.clear();
    }
    buffer.clear();
    return 0;
}

const std::vector<std::string> &GLStreamOut::getConsole() const {
    return glConsoleVec;
}

void GLStreamOut::rm(int n) {
    glConsoleVec.erase(glConsoleVec.begin() + n);
}

GLStreamOut glConsole;
std::ostream glout(&glConsole);


std::streambuf::int_type GLInfo::overflow(std::streambuf::int_type c) {
    if (c != EOF) {
        // Append the character to the buffer
        buffer += c;

        // If the character is a newline, interpret buffer [0] and [1] as a 2-digit number. store that number to int[idx]
        if (c == '\n') {
            int idx = buffer[0] - '0'; // convert char to int
            idx *= 10;
            idx += buffer[1] - '0'; // convert char to int
            glInfoMap[idx] = buffer.substr(2);
            buffer.clear();
        }
    }
    return c;
}

std::streambuf::int_type GLInfo::sync() {
    if (!buffer.empty()) {
        int idx = buffer[0] - '0'; // convert char to int
        idx *= 10;
        idx += buffer[1] - '0'; // convert char to int
        glInfoMap[idx] = buffer.substr(2);
        buffer.clear();
    }
    return 0;
}

const std::map<int, std::string> &GLInfo::getInfo() const {
    return glInfoMap;
}

GLInfo glStatus;
std::ostream glInfoOut(&glStatus);


std::vector<thingHolder> staticPoints;

void addDbgPt(int idx, float xyz[3], float size, float weight) {
    return;
    // I think this may be causing the memory leak, happening @ ~2MB/s.
    //This is the only thing i can think of that's actually creating data every frame,
    //I was careful to destroy or overwrite all other data logging
    staticPoints.insert(staticPoints.begin() + idx, thingHolder(xyz[0], xyz[1], xyz[2], size, weight));
}

std::vector<thingHolder> getDbgPts() {
    return staticPoints;
}

float *getDbgPts(int which) {
    // if which is in the range of the vector, return it, otherwise return the first element
    if (which >= 0 && which < staticPoints.size()) {
        return staticPoints[which];
    } else {
        return staticPoints[0];
    }
}

int getNextPoint(int current) {
    // simply returns the index of the next point, unless the next point would be outside the bounds of the vector, in which case it returns 0
    return current + 1 < staticPoints.size() ? current + 1 : 0;
}

//I did write these, c++ was annoying me my not letting me have the hex
#ifndef FOLDING_REGION_WHY_CPLUSPLUS_WHY
int_fast8_t xx8(char input) {
    return static_cast<int_fast8_t>(input);
}

char xd8(int_fast8_t input) {
    return static_cast<char>(input);
}

int_fast16_t xx16(int input) {
    return static_cast<int_fast16_t>(input);
}

short xd16(int_fast16_t input) {
    return static_cast<short>(input);
}

int_fast32_t xx32(long input) {
    return static_cast<int_fast32_t>(input);
}

long xd32(int_fast32_t input) {
    return static_cast<long>(input);
}

int_fast64_t xx64(long input) {
    return static_cast<int_fast64_t>(input);
}

long xd64(int_fast64_t input) {
    return static_cast<long>(input);
}

std::string xs8(int_fast8_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(value);
    return ss.str();
}

std::string xs16(int_fast16_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(4) << static_cast<int>(value);
    return ss.str();
}

std::string xs32(int_fast32_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(8) << static_cast<int>(value);
    return ss.str();
}

std::string xs64(int_fast64_t value) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(16) << static_cast<int>(value);
    return ss.str();
}


#endif


int8_t doorOpenPercent = 0;


int8_t animateDoor = 3;


int winner;

std::string getUName() {
    const char *username;

#ifdef _WIN32
    username = std::getenv("USERNAME");
#else
    username = std::getenv("USER");
#endif

    if (username != nullptr) {
        return username;
    } else {
        return "Incognito";
    }
}

std::string getDayOfWeek() {
    std::vector<std::string> daysOfWeek = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
    };

    auto now = std::chrono::system_clock::now();
    auto timePoint = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&timePoint);

    return daysOfWeek[tm.tm_wday];
}

int useTimeToSeedRandomToSetWinner() {
    std::random_device rd;
    srand(rd());
    int winnera = rand() % 2;
    return winnera;
}

std::string retWinner() {
    if (winner) {
        return (getUName());
    } else {
        return (getDayOfWeek());
    }
}


//mapping the state to strings for printing

std::map<int, std::string> dbgNormMap = {
    {DBG_NORM_OFF, "None"},
    {ABS_NORM, "Abs(Norm)"},
    {RAW_NORM, "Scaled"},
    {POS_NORM, "Positive Only"},
    {NEG_NORM, "Negative Only"}
};


int nextDbgState() {
    if (dbgNormals == NEG_NORM) {
        return DBG_NORM_OFF;
    } else {
        return dbgNormals + 1;
    }
}

int cardRotState = CARD_ROT_NONE;
int cardRotPercent = 0; //out of 100, but will be scaled
int cardRotSpeed = 1;
int dbgNormals = 0;
uint8_t enabledFaces = 0b00111111;

// this is what enables the normals visualization
void glNormal3fvd(float whyAreMyNormalsBroken[3]) {
    GLfloat normToColors[3];
    float debugWhyAreMyNormalsBroken[3] = {
        whyAreMyNormalsBroken[0], whyAreMyNormalsBroken[1], whyAreMyNormalsBroken[2]
    };
    switch (dbgNormals) {
        case 0:
            glNormal3fv(whyAreMyNormalsBroken);
            break;
        case 1:
            normToColors[0] = abs(whyAreMyNormalsBroken[0]);
            normToColors[1] = abs(whyAreMyNormalsBroken[1]);
            normToColors[2] = abs(whyAreMyNormalsBroken[2]);
            glDisable(GL_LIGHTING);
            break;
        case 2:
            normToColors[0] = (whyAreMyNormalsBroken[0] + 1) / 2;
            normToColors[1] = (whyAreMyNormalsBroken[1] + 1) / 2;
            normToColors[2] = (whyAreMyNormalsBroken[2] + 1) / 2;
            glDisable(GL_LIGHTING);
            break;
        case 3: //just get rid of any values below 0
            normToColors[0] = whyAreMyNormalsBroken[0] > 0 ? whyAreMyNormalsBroken[0] : 0;
            normToColors[1] = whyAreMyNormalsBroken[1] > 0 ? whyAreMyNormalsBroken[1] : 0;
            normToColors[2] = whyAreMyNormalsBroken[2] > 0 ? whyAreMyNormalsBroken[2] : 0;
            glDisable(GL_LIGHTING);
            break;
        case 4: //just get rid of any values above 0
            normToColors[0] = whyAreMyNormalsBroken[0] < 0 ? -1 * whyAreMyNormalsBroken[0] : 0;
            normToColors[1] = whyAreMyNormalsBroken[1] < 0 ? -1 * whyAreMyNormalsBroken[1] : 0;
            normToColors[2] = whyAreMyNormalsBroken[2] < 0 ? -1 * whyAreMyNormalsBroken[2] : 0;
            glDisable(GL_LIGHTING);
            break;
        default:
            glNormal3fv(whyAreMyNormalsBroken);
            break;
    }
    if (dbgNormals != 0) {
        glColor3fv(normToColors);
    }
}


bool selecting = false;
int xClick;
int yClick;

float globAmb[4] = {0.2, 0.2, 0.2, 1.0};

float hallBnlF[3] = {0, 0, -2};
float halltfrF[3] = {10, 4, 2};

//Coord roomBnl = Coord(halltfrF[0], 0, 3*hallBnlF[2]);
float roomBnlF[3] = {10, 0, -6};
float roomtfrF[3] = {20, 4, 6};
bool useCollision = true;
float moveSpeed = 0.5f;
float cardDist;

bool bufferPeeking = false;
bool windowFocused = false;
bool selectLock = false;
bool selectLockingEnabled = false;


int animatingBlinds = 3;
int blindsProgress = 0;



//                  __      ______                     __
//    ______________\ \    / ____/___  ____  _________/ / _________  ____
//   / ___/ ___/ ___/\ \  / /   / __ \/ __ \/ ___/ __  / / ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /___/ /_/ / /_/ / /  / /_/ /_/ /__/ /_/ / /_/ /
// /____/_/   \___/    \_\____/\____/\____/_/   \__,_/(_)___/ .___/ .___/
//                                                         /_/   /_/


// #include "globals.h"
#include <cmath>

#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif

// #include "Coord.h"

#include <cmath>

#define DEBUG

#include <algorithm>
#include <iostream>
#include <random>
#include <iomanip>
#include <map>
#include <sstream>

void mapInsert(std::map<int, std::string>* mapthing, std::vector<std::string> vec, int indexInsert) {
    for (int i = 0; i < vec.size(); i++) {
        mapthing->insert(std::pair<int, std::string>(indexInsert + i, vec[i]));
    }

}

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

Coord::Coord(std::string hexString) {
    std::stringstream Xstr (hexString.substr(0, 20));
    std::stringstream Ystr (hexString.substr(20, 20));
    std::stringstream Zstr (hexString.substr(40, 20));
    // float outX, outY, outZ;
    Xstr >> std::hexfloat >> X;
    Ystr >> std::hexfloat >> Y;
    Zstr >> std::hexfloat >> Z;
}

std::string Coord::toHexString(char delim) { //remember, comma-delimited
    std::stringstream stream;
    stream << std::hexfloat << X << delim << std::hexfloat << Y << delim << std::hexfloat << Z;
    return stream.str();
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

Coord Coord::wrap(Coord maxes, Coord mins, Coord add) {
    Coord result = *this + add;

    if (result.X > maxes.X) {
        result.X = mins.X + (result.X - maxes.X);
    } else if (result.X < mins.X) {
        result.X = maxes.X - (mins.X - result.X);
    }

    if (result.Y > maxes.Y) {
        result.Y = mins.Y + (result.Y - maxes.Y);
    } else if (result.Y < mins.Y) {
        result.Y = maxes.Y - (mins.Y - result.Y);
    }

    if (result.Z > maxes.Z) {
        result.Z = mins.Z + (result.Z - maxes.Z);
    } else if (result.Z < mins.Z) {
        result.Z = maxes.Z - (mins.Z - result.Z);
    }

    return result;
}

Coord Coord::wrap(float maxe, float min, Coord add) {
    Coord result = *this + add;

    if (result.X > maxe) {
        result.X = min + (result.X - maxe);
    } else if (result.X < min) {
        result.X = maxe - (min - result.X);
    }

    if (result.Y > maxe) {
        result.Y = min + (result.Y - maxe);
    } else if (result.Y < min) {
        result.Y = maxe - (min - result.Y);
    }

    if (result.Z > maxe) {
        result.Z = min + (result.Z - maxe);
    } else if (result.Z < min) {
        result.Z = maxe - (min - result.Z);
    }

    return result;
}

Coord Coord::radiansToDegrees() const {
    float herePi = 3.14159265358979323846;
        return {(X * 180 / herePi),(Y * 180 / herePi),(Z * 180 / herePi)};

}

Coord Coord::degToRad() {
    float herePi = 3.14159265358979323846;
    return {(X * herePi / 180),(Y * herePi / 180),(Z * herePi / 180)};



}

Coord Coord::dirVecToRad() const {
        float pitch = atan2(Y, Z);
        float yaw = atan2(X, Z);
        return Coord(pitch, yaw, 0);
}

Coord Coord::radiansToDirVec() const {
    float pitch = X;
    float yaw = Y;
    return Coord(sin(pitch) * cos(yaw), sin(pitch) * sin(yaw), cos(pitch));
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

void Coord::debug(int idx, float s, float w) {
    addDbgPt(idx, new float[3]{X, Y, Z}, s, w);
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



//                  __      ______      __           ____        __
//    ______________\ \    / ____/___  / /___  _____/ __ \____ _/ /_____ _ _________  ____
//   / ___/ ___/ ___/\ \  / /   / __ \/ / __ \/ ___/ / / / __ `/ __/ __ `// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /___/ /_/ / / /_/ / /  / /_/ / /_/ / /_/ /_/ // /__/ /_/ / /_/ /
// /____/_/   \___/    \_\____/\____/_/\____/_/  /_____/\__,_/\__/\__,_(_)___/ .___/ .___/
//                                                                          /_/   /_/

// #include "ColorData.h"




bool isColorCloseEnough(ColorData color1, ColorData color2, float closeness) {
    if (abs(color1.R - color2.R) < closeness &&
        abs(color1.G - color2.G) < closeness &&
        abs(color1.B - color2.B) < closeness) {
        return true;
        }
    return false;
}



//                  __        ___       __    __  _
//    ______________\ \      / (_)___ _/ /_  / /_(_)___  ____ _ _________  ____
//   / ___/ ___/ ___/\ \    / / / __ `/ __ \/ __/ / __ \/ __ `// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \  / / / /_/ / / / / /_/ / / / / /_/ // /__/ /_/ / /_/ /
// /____/_/   \___/    \_\/_/_/\__, /_/ /_/\__/_/_/ /_/\__, (_)___/ .___/ .___/
//                            /____/                  /____/     /_/   /_/


// #include "lighting.h"


Light::Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diff, ColorData spec) {
    whichLight = whichLighta;
    lightAmb = amb;
    lightDiff = diff;
    lightSpec = spec;
    lightPos = pos;
}

Light::Light(lightNum whichLighta, ColorData pos, ColorData amb, ColorData diffspec) {
    whichLight = whichLighta;
    lightAmb = amb;
    lightDiff = diffspec;
    lightSpec = diffspec;
    lightPos = pos;
}

void Light::setup() {
    glLightfv(whichLight, GL_AMBIENT, lightAmb);
    glLightfv(whichLight, GL_DIFFUSE, lightDiff);
    glLightfv(whichLight, GL_SPECULAR, lightSpec);
    glLightfv(whichLight, GL_POSITION, lightPos);
}

void Light::enable() const {
    if(enabled) {
        glEnable(whichLight);
    }
}

void Light::disable() const {
    glDisable(whichLight);
}

void Light::lightswitch() {
    enabled = !enabled;
}

void Light::lightswitch(bool onoff) {
    enabled = onoff;
}

Light::operator lightNum() const {
    return whichLight; //returns, for example, GL_LIGHT0
}

Light::Light() = default;


Spotlight::Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, ColorData dir,
                     float cutoff, float exponent): Light(whichLight, pos, amb, diff, spec) {
    spotDir = dir;
    spotAttenuation = exponent;
    spotAngle = cutoff;
}

Spotlight::Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, Coord dir,
                     float cutoff, float exponent): Light(whichLight, pos, amb, diff, spec) {
    spotDir = dir;
    spotAttenuation = exponent;
    spotAngle = cutoff;
}

Spotlight::Spotlight() = default;

Spotlight::Spotlight(Light i, Coord dir, float cutoff, float exponent): Light(i.whichLight, i.lightPos, i.lightAmb,
                                                                              i.lightDiff, i.lightSpec) {
    spotDir = dir;
    spotAttenuation = exponent;
    spotAngle = cutoff;
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


Material::Material(ColorData spec, ColorData amb, ColorData diff, ColorData emission, float shine) {
    matSpec = spec;
    matAmb = amb;
    matEmission = emission;
    matDiff = diff;
    matShine = shine;
}

Material::Material(ColorData spec, ColorData amb, ColorData diff, float shine) {
    matSpec = spec;
    matAmb = amb;
    matEmission = ColorData(0.0f, 0.0f, 0.0f, 0.0f);
    matDiff = diff;
    matShine = shine;
}

void Material::apply() {
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec); //GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmb); //GL_FRONT_AND_BACK, GL_AMBIENT, matAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, matEmission); //GL_FRONT_AND_BACK, GL_EMISSION, matEmission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff); //GL_FRONT_AND_BACK, GL_DIFFUSE, matDiff);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShine); //GL_FRONT_AND_BACK, GL_SHININESS, matShine);
}

//Material Library:
#ifndef FOLDING_REGION_MaterialLibrary

Material wallMat = Material(
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    10.0f);

Material floorMat = Material( //pale brown color
    ColorData(0.8f, 0.6f, 0.4f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.6f, 0.4f, 1.0f),
    33.0f); //just a bit glossy


Material ceilingMat = Material( //matte cream color
    ColorData(0.9f, 0.9f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.9f, 0.9f, 0.8f, 1.0f),
    5.0f);

Material lampMat = Material( //emissive golden color.
    ColorData(1.0f, 0.9f, 0.3f, 1.0f),
    ColorData(0.2f, 0.2f, 0.1f, 1.0f),
    ColorData(1.0f, 0.9f, 0.3f, 1.0f),
    ColorData(1.0f, 0.9f, 0.3f, 1.0f),
    5.0f);

Material buttonMat = Material( //emissive bluish color
    ColorData(0.3f, 0.9f, 0.9f, 1.0f),
    ColorData(0.1f, 0.2f, 0.3f, 1.0f),
    ColorData(0.0f, 0.3f, 1.0f, 1.0f),
    ColorData(0.0f, 0.3f, 0.8f, 1.0f),
    5.0f);

Material cardMat = Material(
       ColorData(0.3f,0.3f,0.3f, 1.0f),
        ColorData(0.05f, 0.05f, 0.1f, 1.0f),
        ColorData(0.15f, 0.25f, 0.8f, 1.0f),
        10.0f);

Material castIronMat = Material( // nearly black, metallic
    ColorData(1.0f, 1.0f, 1.0f, 1.0f),
    ColorData(0.0f, 0.0f, 0.0f, 1.0f),
    ColorData(0.1f, 0.1f, 0.1f, 1.0f),
    10.0f);

//shiny dark brown texture for table
Material tableMat = Material(
    ColorData(1.0f, 1.0f, 0.5f, 1.0f),
    ColorData(0.1f,0.1f,0.1f, 1.0f),
    ColorData(Coord(149, 69, 53)/255, 1.0f),
    10.0f);

Material superShinySteel = Material(
    ColorData(0.8f, 1.0f, 1.0f, 1.0f),
    // ColorData(scrollVar, scrollVar, scrollvar, 1.0f),
    ColorData(0.1f, 0.1f, 0.1f, 1.0f),
    ColorData(0.2f,0.2f,0.2f, 1.0f),
    10.0f);

Material matteConcrete = Material(
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    5.0f);

Material shinyRed = Material(
    ColorData(0.9f, 0.5f, 0.5f, 1.0f), //specular
    ColorData(0.2f, 0.2f, 0.2f, 1.0f), //ambient
    ColorData(1.0f, 0.0f, 0.0f, 1.0f), //diffuse
    10.0f); //shininess

Material shinyGreen = Material(
    ColorData(1.0f, 1.0f, 1.0f, 1.0f), //specular
    ColorData(0.2f, 0.2f, 0.2f, 1.0f), //ambient
    ColorData(0.0f, 1.0f, 0.0f, 1.0f), //diffuse
    100.0f); //shininess

Material shinyBlue = Material(
    ColorData(0.4f, 0.4f, 1.0f, 1.0f), //specular
    ColorData(0.2f, 0.2f, 0.2f, 1.0f), //ambient
    ColorData(0.0f, 0.0f, 1.0f, 1.0f), //diffuse
    10.0f); //shininess


#endif


//globally accessible object roomLight;
Light roomLight = Light(
    Light::LIGHT1,
    ColorData(Coord(15, 3, 0), 1.0), //pos
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //amb
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //diff
    ColorData(1.0f, 1.0f, 1.0f, 1.0f) //spec
);

Light hallLight = Light(
    Light::LIGHT5,
    ColorData(Coord(15, 3, 0), 1.0), //pos
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //amb
    ColorData(1.0f, 0.9f, 0.85f, 1.0f), //diff
    ColorData(1.0f, 1.0f, 1.0f, 1.0f) //spec
);


Light brightWhite = Light(
    Light::LIGHT3,
    ColorData(Coord(0, 0, 0), 1.0),
    ColorData(0.2f, 0.2f, 0.2f, 1),
    ColorData(1.0f, 1.0f, 1.0f, 1.0f),
    ColorData(1.0f, 1.0f, 1.0f, 1.0f));

Light brightRed = Light(
    Light::LIGHT4,
    ColorData(Coord(0, 10, 0), 1.0),
    ColorData(0.2f, 0.05f, 0.05f, 1),
    ColorData(1.0f, 0.2f, 0.2f, 1.0f),
    ColorData(1.0f, 0.2f, 0.2f, 1.0f));

Spotlight headLamp = Spotlight(
    brightWhite,
    Coord(1, 0, 0),
    15.0f,
    10.0f);



float sunScale = 0.5;
Light sunLight = Light( //directional sunlight, high in the sky
    Light::LIGHT2,
    ColorData(1.0f, 1.0f, 1.0f, 0.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1),
    ColorData(sunScale, sunScale, sunScale, 1.0f),
    ColorData(sunScale, sunScale, sunScale, 1.0f));



//                  __        __         ______ _    ______
//    ______________\ \      / /   ___  / __/ /| |  / / __ \ _________  ____
//   / ___/ ___/ ___/\ \    / /   / _ \/ /_/ __/ | / / /_/ // ___/ __ \/ __ \
//  (__  ) /  / /__   \ \  / /___/  __/ __/ /_ | |/ / ____// /__/ /_/ / /_/ /
// /____/_/   \___/    \_\/_____/\___/_/  \__/ |___/_/   (_)___/ .___/ .___/
//                                                            /_/   /_/

// #include "globals.h"
// #include "LeftVP.h"

#include <iostream>
#include <map>

// #include "ColorData.h"

extern float rVPColor[];
extern ColorData rVPColorData;
extern ColorData solarizedBG;
extern ColorData solarizedText;
float conHeightPercent = 0.3;
extern std::map<int, std::string> debugMap;
extern int lVportW;
extern int height;

int mNum = 0;
float rposX = 3;

void processConsoleMsgs() {
    for (int i = glConsole.size() - 1; i >= 0; --i) {
        int messageHeight = 3 + (glConsole.size() - i) * 15;

        if (messageHeight > conHeightPercent * height) {
            glConsole.rm(i);
        } else {
            glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
            glRasterPos3f(rposX, messageHeight, -1.0);

            for (int8_t c: glConsole[i]) {
                if (c <= 0x7f && c != 0x00 && c != 0x0a) {
                    //just don't print the rest of the line if there is an illegal character
                    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
                }
            }
        }
    }
}

void drawLeft() {
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    //draw background with bkgColor; must be a rectangle, not glclearcolor
    glColor4f(solarizedBG.R, solarizedBG.G, solarizedBG.B, solarizedBG.A);
    // vertex array for the background
    GLint vertices[] = {
        0, 0,
        lVportW, 0,
        lVportW, height,
        0, height
    };
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, vertices);
    glDrawArrays(GL_QUADS, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    //draw thick solarized orange line along the right edge
    glLineWidth(5.0f);
    glColor4f(0.86275, 0.19608, 0.18431, 1.0);
    glBegin(GL_LINES);
    glVertex2i(lVportW, 0);
    glVertex2i(lVportW, height);
    glEnd();

    //draw thick horizontal solarized orange line 20% from the bottom
    glLineWidth(2.50f);
    glBegin(GL_LINES);
    glVertex2i(0, conHeightPercent * height);
    glVertex2i(lVportW, conHeightPercent * height);
    glEnd();
    glLineWidth(1.0f);

    glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
    for (const auto &pair: debugMap) {
        glRasterPos3f(3.0, (pair.first + 1) * 15, -1.0);
        std::for_each(pair.second.begin(), pair.second.end(),
                      [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
    }
    processConsoleMsgs();
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void setupLeft() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glViewport(0, 0, lVportW, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, lVportW, 0, height, -10, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}





//                  __      __  __    _
//    ______________\ \    / /_/ /_  (_)___  ____ ______ _________  ____
//   / ___/ ___/ ___/\ \  / __/ __ \/ / __ \/ __ `/ ___// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /_/ / / / / / / / /_/ (__  )/ /__/ /_/ / /_/ /
// /____/_/   \___/    \_\__/_/ /_/_/_/ /_/\__, /____(_)___/ .___/ .___/
//                                        /____/          /_/   /_/

// #include "globals.h"
// #include "things.h"
// #include "Coord.h"
#include <map>

#include <iostream>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif


extern std::map<int, std::string> debugMap;


//Window Blinds Routines
#ifndef FOLDING_REGION_BLINDS

Blinds::Blinds(float width, float height, float depth, float pitchAngle, float closedFactor) {
    matSpecBlinds = ColorData(0.5, 0.5, 0.5, 1.0);
    matShineBlinds[0] = 50.0f;
    matAmbAndDifBlinds = ColorData(0.9, 0.9, 0.89, 1.0);

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
    glRotatef(pitchAngle, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(depth, bladeHeight, width);
    glutSolidCube(1.0);
    glPopMatrix();
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

    float bladeSpacing = depth * closedFactor + bladeHeight / 2;
    float topBladeHeight = height - bladeSpacing;
    glPushMatrix();
    for (int i = 0; i < numBlades; i++) {
        // bool isBladeOpened = closedFactor * numBlades < i; // NOLINT(*-narrowing-conversions)
        glPushMatrix();
        glTranslatef(0.0, topBladeHeight - ((i - 1) * bladeSpacing), 0.0);
        drawBlade();
        glPopMatrix();
    }
    glPopMatrix();

    drawDbgPoints(dbg);

    if (debug_string_add_map_ != nullptr) {
        (*debug_string_add_map_)[60-17] = "Blinds State: " + std::to_string(closedFactor);
        (*debug_string_add_map_)[60-18] = "Blade Spacing: " + std::to_string(bladeSpacing);
        (*debug_string_add_map_)[60-19] = "Num Blades: " + std::to_string(numBlades);
    }
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
#endif


//Drawing a 3D Debug Axis
#ifndef FOLDING_REGION_DEBUG3DX

Debug3Dx::Debug3Dx(Coord *position, float size, float weight) {
    this->size = size;
    this->weight = weight;
    this->position = *position;
}

Debug3Dx::Debug3Dx(Coord position, float size, float weight) {
    this->position = position;
    this->size = size;
    this->weight = weight;
}

Debug3Dx::Debug3Dx(float inStuff[5]) {
    this->position = {inStuff[0], inStuff[1], inStuff[2]};
    this->size = inStuff[3];
    this->weight = inStuff[4];
}

void Debug3Dx::draw() const {
    //set line width to weight
    glLineWidth(weight);
    glPushMatrix();

    // X axis
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X - size, position.Y, position.Z);
    glVertex3f(position.X + size, position.Y, position.Z);
    glEnd();

    //y axis
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X, position.Y - size, position.Z);
    glVertex3f(position.X, position.Y + size, position.Z);
    glEnd();

    //z axis
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(position.X, position.Y, position.Z - size);
    glVertex3f(position.X, position.Y, position.Z + size);
    glEnd();

    glPopMatrix();


    glLineWidth(1);
}

void Debug3Dx::draw(Coord pos, float length, float weight) const {
    //create a new Debug3Dx object and draw it, then delete it
    Debug3Dx(pos, length, weight).draw();
    //delete it when done
}

void drawXZxGridlines(float range) {
    glColor3f(0.7f, 0.7f, 0.7f); // Set color to light gray
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x0404);
    for (int i = -range; i <= range; ++i) { // NOLINT(*-narrowing-conversions)
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

#endif

void drawBMPStr(std::string str, void *font) {
    for (int8_t c: str) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c);
    }
}

void drawFlatPlaneYZ(Coord corner1, Coord corner2, int numSubDivisions) { //plane in the YZ plane
    float yStep = (corner2.Y - corner1.Y) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float zStep = (corner2.Z - corner1.Z) / numSubDivisions; // NOLINT(*-narrowing-conversions)

    // Draw the grid of triangle strips
    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X, corner1.Y + i * yStep, corner1.Z + j * zStep);
            glVertex3f(corner1.X, corner1.Y + (i + 1) * yStep, corner1.Z + j * zStep);
        }
        glEnd();
    }
}

void drawFlatPlaneXY(Coord corner1, Coord corner2, int numSubDivisions) { //plane in the XY plane
    float xStep = (corner2.X - corner1.X) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float yStep = (corner2.Y - corner1.Y) / numSubDivisions; // NOLINT(*-narrowing-conversions)

    // Draw the grid of triangle strips
    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X + i * xStep, corner1.Y + j * yStep, corner1.Z);
            glVertex3f(corner1.X + (i + 1) * xStep, corner1.Y + j * yStep, corner1.Z);
        }
        glEnd();
    }
}

void drawFlatPlaneXZ(Coord corner1, Coord corner2, int numSubDivisions) { //plane in the XZ plane
    float xStep = (corner2.X - corner1.X) / numSubDivisions; // NOLINT(*-narrowing-conversions)
    float zStep = (corner2.Z - corner1.Z) / numSubDivisions; // NOLINT(*-narrowing-conversions)

    // Draw the grid of triangle strips
    for (int i = 0; i < numSubDivisions; ++i) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j <= numSubDivisions; ++j) {
            glVertex3f(corner1.X + i * xStep, corner1.Y, corner1.Z + j * zStep);
            glVertex3f(corner1.X + (i + 1) * xStep, corner1.Y, corner1.Z + j * zStep);
        }
        glEnd();
    }
}

void drawPlane(Coord c1, Coord c2, Coord normalVec, int numSubDivisions) {
    //draw the flat plane with the correct dimensions and location

    //figure out how the final plane will need to be rotated based on the corners;
    float debuga[3] =  {c1.X, c1.Y, c1.Z};
    float debugb[3] = {c2.X, c2.Y, c2.Z};
    float dbgn[3] = {normalVec.X, normalVec.Y, normalVec.Z};

    // XZ plane: CORRECT!
    if (abs(c1.Y - c2.Y) <= 0.001) {

        glPushMatrix();
        glNormal3fvd(normalVec);
        // glTranslatefv(c1);
        // drawFlatPlaneXZ(Coord(), c2 - c1, numSubDivisions);
        drawFlatPlaneXZ(c1, c2, numSubDivisions);
        glPopMatrix();
    }

    // XY plane:x
    if (abs(c1.Z - c2.Z) <= 0.001) {
        //here originally
        glPushMatrix();
        glNormal3fvd(normalVec);
        // glTranslatefv(c1);
        // drawFlatPlaneXY(Coord(), c2 - c1, numSubDivisions);
        drawFlatPlaneXY(c1, c2, numSubDivisions);
        glPopMatrix();
    }

    //YZ plane: rotate 90 degrees about the y axis
    if (abs(c1.X - c2.X) <= 0.001) {
        //here originally
        glPushMatrix();
        // glTranslatefv(c1);
        glNormal3fvd(normalVec);
        drawFlatPlaneYZ(c1, c2, numSubDivisions);
        glPopMatrix();
    }
    if(dbgNormals!=0) {
        glEnable(GL_LIGHTING);
    }
}

void cubeGLfrom2Points(Coord bnl, Coord tfr, uint8_t mode) {
    //draw the cube with the correct dimensions and location
    glPushMatrix();
    glTranslatefv((tfr-bnl)/2+bnl);
    glScalefv(tfr-bnl);
if(mode == WiREFRAME)
    glutWireCube(1.0);
else if(mode == SOLID)
    glutSolidCube(1.0);
    glPopMatrix();
}


//manually building the coordinate vectors for normalse:


Coord farXpositive = Coord(1.0f, 0.0f, 0.0f);//Coord farXpositive = Coord(1, 0, 0);
Coord topYpositive = Coord(0.0f, 1.0f, 0.0f);//Coord topYpositive = Coord(0, 1, 0);
Coord rightZpositive = Coord(0.0f, 0.0f, 1.0f);//Coord rightZpositive = Coord(0, 0, 1);
Coord nearXnegative = Coord(-1.0f, 0.0f, 0.0f);//farXpositive * -1;            //Coord(0, 0, 0);
Coord bottomYnegative = Coord(0.0f, -1.0f, 0.0f);//topYpositive * -1;          //Coord(0, 0, 0);
Coord leftZnegative = Coord(0.0f, 0.0f, -1.0f);//rightZpositive * -1;          //Coord(0, 0, 0);

void drawInsideOutCube(Coord bnl, Coord tfr, int numSubDiv, uint8_t whichFaces) {
    float debuga[3] =  {bnl.X, bnl.Y, bnl.Z};
    float debugb[3] = {tfr.X, tfr.Y, tfr.Z};
    if (whichFaces >> 5 & 1 && enabledFaces >> 5 & 1){ //far +X
        // int a = whichFaces >> 5 & 1;
        drawPlane(Coord(tfr.X, bnl.Y, bnl.Z), tfr, nearXnegative, numSubDiv);
    }
    if (whichFaces >> 4 & 1  && enabledFaces >> 4 & 1){ //top +Y
        drawPlane(Coord(bnl.X, tfr.Y, bnl.Z), tfr, bottomYnegative, numSubDiv);
    }
    if (whichFaces >> 3 & 1 && enabledFaces >> 3 & 1){ //right +Z
        drawPlane(Coord(bnl.X, bnl.Y, tfr.Z), tfr, leftZnegative, numSubDiv);
    }
    if (whichFaces >> 2 & 1 && enabledFaces >> 2 & 1){ //near -X
        drawPlane(bnl, Coord(bnl.X, tfr.Y, tfr.Z), farXpositive, numSubDiv);
    }
    if (whichFaces >> 1 & 1 && enabledFaces >> 1 & 1){ //bottom
        drawPlane(bnl, Coord(tfr.X, bnl.Y, tfr.Z), topYpositive, numSubDiv);
    }
    if (whichFaces & 1 && enabledFaces & 1){ //left
        drawPlane(bnl, Coord(tfr.X, tfr.Y, bnl.Z), rightZpositive, numSubDiv);
    }


}


void cubeOfPlanes(Coord bnl, Coord tfr, int numSubDiv, int insideOut, uint8_t whichFaces) {
    /*draw the cube of planes with the correct dimensions and location
    whichFaces is a bitfield that determines which faces to draw; 1 is draw, 0 is don't draw
    1: front (YZ - X+), 2: top (XZ - Y+), 3: right (XY - Z+)
    4: back (YZ - X-), 5: bottom (XZ - Y-), 6: left (XY - Z-)*/
    //flat shading
    if(insideOut == -1) {
        drawInsideOutCube(bnl, tfr, numSubDiv, whichFaces);
        return;
    }

    // glPushMatrix();
    if (whichFaces >> 5 & 1 && enabledFaces >> 5 & 1){ //far +X
        // int a = whichFaces >> 5 & 1;
            glNormal3fvd(farXpositive);
        	glPushMatrix();
        	drawPlane(Coord(tfr.X, bnl.Y, bnl.Z), tfr, farXpositive, numSubDiv); //Coord(-insideOut, 0, 0),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces >> 4 & 1  && enabledFaces >> 4 & 1){ //top +Y
            // glNormal3fvd(topYpositive);
            glPushMatrix();
            drawPlane(Coord(bnl.X, tfr.Y, bnl.Z), tfr, topYpositive, numSubDiv); //Coord(0, -insideOut, 0),  * insideOut
            glPopMatrix();
    }
    if (whichFaces >> 3 & 1 && enabledFaces >> 3 & 1){ //right +Z
            // glNormal3fvd(rightZpositive);
        	glPushMatrix();
        	drawPlane(Coord(bnl.X, bnl.Y, tfr.Z), tfr, rightZpositive, numSubDiv); //Coord(0, 0, -insideOut),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces >> 2 & 1 && enabledFaces >> 2 & 1){ //near -X
            // glNormal3fvd(nearXnegative);
        	glPushMatrix();
        	drawPlane(bnl, Coord(bnl.X, tfr.Y, tfr.Z), nearXnegative, numSubDiv); //Coord(-insideOut, 0, 0),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces >> 1 & 1 && enabledFaces >> 1 & 1){ //bottom -Y
            // glNormal3fvd(bottomYnegative);
        	glPushMatrix();
        	drawPlane(bnl, Coord(tfr.X, bnl.Y, tfr.Z), bottomYnegative, numSubDiv); //Coord(0, -insideOut, 0),  * insideOut
        	glPopMatrix();
    }
    if (whichFaces & 1 && enabledFaces & 1){ //left -Z
            // glNormal3fvd(leftZnegative);
        	glPushMatrix();
        	drawPlane(bnl, Coord(tfr.X, tfr.Y, bnl.Z), leftZnegative, numSubDiv); //Coord(0, 0, -insideOut),  * insideOut
        	glPopMatrix();
    }

    // glPopMatrix();
    if(dbgNormals!=0) {
        glEnable(GL_LIGHTING);
    }

}


void testConeA(float height, ColorData colorThing) {
    float baseR = height / 2;
    glPushMatrix();
    glTranslatef(0, 0, -height);
    glColor4f(colorThing.R, colorThing.G, colorThing.B, colorThing.A);
    glutSolidCone(baseR, height, 20, 20);
    glColor3f(0, 0, 0);
    glLineWidth(2.0f);
    glutWireCone(baseR, height, 20, 20);
    glLineWidth(1.0f);
    glPopMatrix();
}
void testConeCrot(Coord angle, ColorData coloraaaa) {  //add info to debugMap
    glPushMatrix();
    glRotatef(angle.Y, 0, 1, 0);
    glRotatef(angle.X, 1, 0, 0);
    // glRotatef(-angle.Z, 0, 0, 1);
    testConeA(1, coloraaaa);
    glPopMatrix();
}
void testInRightPlace(Camera cam) { // tests the cone originating from the camera
    glPushMatrix();
    glScalef(1, 1, -1);
    glTranslatef(0, 5,0);
    testConeCrot(Coord(D(cam.ang.X), D(cam.ang.Y-PI/2), 0),
        ColorData(0.0f, 1.0f, 0.0f, 0.3f));
    glPopMatrix();
}
void testDrawingCubes() {
    glEnable(GL_BLEND);
    glColor4f(1, 0, 0, 0.3f);
    cubeOfPlanes(Coord(0, 0, 0), Coord(1, 1, 1), 10, 1, 0b100100); //front and back

    glColor4f(0, 0, 1, 0.3f);
    cubeOfPlanes(Coord(0, 0, 0), Coord(1, 1, 1), 10, 1, 0b010010); //top and bottom

    glColor4f(0, 1,0, 0.3f);
    cubeOfPlanes(Coord(0, 0, 0), Coord(1, 1, 1), 10, 1, 0b001001); //Left and rightX
    glDisable(GL_BLEND);

}
void windowTest() {
    Coord wall1a = Coord(0, 0, 6);
    Coord wall1B = Coord(7, 6, 6.5);
    ColorData wall1Color = ColorData(0x6446AD,  1.0);

    Coord wall2a = Coord(1, 0, 4    );
    Coord wall2B = Coord(6, 5, 4.5);
    ColorData wall2Color = ColorData(0x2AAD8F, 1.0);

    Coord window1a = Coord(2.5, 1.5, 3.99);
    Coord window1b = Coord(3.5, 2.5, 4.501);
    Coord window1Color = ColorData(0x02BCEA, 0.0);

    Coord window2a = Coord(2.5, 1.5, 4.01);
    Coord window2b = Coord(3.5, 2.5, 4.502);

    Coord window2Color = ColorData(0x02BCEA, 1);

    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);

    //wallMat 1 should be untouched
    glColor4fv(wall1Color);
    cubeGLfrom2Points(wall1a, wall1B);



    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    //window 1 will use its transparency to cut a hole in the wallMat
    glColor4fv(window1Color);
    cubeGLfrom2Points(window1a, window1b);


    //usng a different blend function to make the object show up in the hole:

    //wallMat 2 should have a hole through it
    glColor4fv(wall2Color);
    cubeGLfrom2Points(wall2a, wall2B);

    glBlendFunc(GL_ONE, GL_ZERO);
    //and this should be a translucent blue object within the hole in wallMat 2
     glColor4fv(window2Color);
     cubeGLfrom2Points(window2a, window2b);

//    glDisable(GL_BLEND);

}




//                  __      ______
//    ______________\ \    / ____/___ _____ ___  ___  _________ _ _________  ____
//   / ___/ ___/ ___/\ \  / /   / __ `/ __ `__ \/ _ \/ ___/ __ `// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /___/ /_/ / / / / / /  __/ /  / /_/ // /__/ /_/ / /_/ /
// /____/_/   \___/    \_\____/\__,_/_/ /_/ /_/\___/_/   \__,_(_)___/ .___/ .___/
//                                                                 /_/   /_/

// #include "Camera.h"
// #include "globals.h"

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

// #include "things.h"


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


void Camera::setPitchYaw() {
    Coord dirVec = (tgt - pos);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    ang = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
}

// Coord calcPitchYaw(Coord position, Coord target) {
//     Coord targetI = target + Coord(0.001, 0.001, 0.001);
//     Coord dirVec = (targetI - position);
//     dirVec = dirVec / sqrt(dirVec.pow(2).sum());
//     Coord rv = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
//     return rv;
// }

// Coord calcPitchYaw(Coord position, Coord target) {
//     Coord dirVec = (target - position);
//     dirVec = dirVec / sqrt(dirVec.pow(2).sum());
//
//     float pitch = atan2(dirVec.Y, sqrt(dirVec.X * dirVec.X + dirVec.Z * dirVec.Z));
//     float yaw = atan2(dirVec.X, dirVec.Z);
//
//     return Coord(pitch, yaw, 0);
// }


Coord calcPitchYaw(Coord position, Coord target) {
    Coord targetI = target + Coord(0.001, 0.001, 0.001);
    Coord dirVec = (targetI - position);
    dirVec = dirVec / sqrt(dirVec.pow(2).sum());
    Coord rv = Coord(atan2(dirVec.X, dirVec.Z), asin(dirVec.Y), 0);
    dirVec = Coord(0, 0, 0);
    return Coord(0.0001, 0.0001, 0);
    // return rv;
}



Coord calcTarget(Coord position, Coord pitchYaw) {
    float dirX = cos(pitchYaw.X) * cos(pitchYaw.Y);
    float dirY = sin(pitchYaw.Y);
    float dirZ = sin(pitchYaw.X) * cos(pitchYaw.Y);
    return position + Coord(dirX, dirY, dirZ);
}



Coord roomBottomNearLeft = Coord(roomBnlF);
Coord roomTopFarRight = Coord(roomtfrF);
Coord hallBottomNearLeft = Coord(hallBnlF);
Coord hallTopFarRight = Coord(halltfrF);
//collision:

//XZ Collision:





int Camera::whichSpace(Coord posi) {
    if (posi.X > roomBottomNearLeft.X+moveSpeed/2 && posi.X < roomTopFarRight.X &&
        posi.Y > roomBottomNearLeft.Y && posi.Y < roomTopFarRight.Y &&
        posi.Z > roomBottomNearLeft.Z && posi.Z < roomTopFarRight.Z) {

        whichPlace = IN_ROOM;
    }
    if (posi.X > hallBottomNearLeft.X && posi.X < hallTopFarRight.X-moveSpeed/2 &&
        posi.Y > hallBottomNearLeft.Y && posi.Y < hallTopFarRight.Y &&
        posi.Z > hallBottomNearLeft.Z && posi.Z < hallTopFarRight.Z) {
        whichPlace = IN_HALL;
    }
    std::string place = whichPlace == IN_ROOM ? "Room" : "Hall";
    debugMap[60-21] = "Location: " + place;
    return whichPlace;
}

int Camera::collisionYZ(Coord pos) {

    float roomTfrfH[3] = {roomTopFarRight.X, roomTopFarRight.Y, roomTopFarRight.Z};
    float roomBnlfH[3] = {roomBottomNearLeft.X, roomBottomNearLeft.Y, roomBottomNearLeft.Z};
    float hallTfrfH[3] = {hallTopFarRight.X, hallTopFarRight.Y, hallTopFarRight.Z};
    float hallBnlfH[3] = {hallBottomNearLeft.X, hallBottomNearLeft.Y, hallBottomNearLeft.Z};
    //min X check for hall:
    int coll = whichSpace(pos); //1 means in hall
    if (pos.X < hallBottomNearLeft.X) {
        debugMap[60-22] = "Near YZ collision";
        // glout << "Near YZ collision" << std::endl;
        return COLL_NEAR_YZ;
    }
    float  rttfr= roomTopFarRight.X;
    double rtfrExtend = rttfr;
    bool doorWalkthrough = pos.X >= hallTopFarRight.X-moveSpeed/2;
    bool doorOpen = doorOpenPercent < 90;
    bool inHall = coll == IN_HALL;
    bool inRm = coll == IN_ROOM;
    bool leftXRoom = pos.X < roomBottomNearLeft.X;
    bool inHallByZ = hallTopFarRight.Z > pos.Z > hallBottomNearLeft.Z;

    if ((inHall && doorWalkthrough && doorOpen) ||
        (inRm && doorOpen && (leftXRoom && inHallByZ) )) {
        debugMap[60-22] = "Door collision";

            return COLL_DOOR;
    }
    bool throughFarWall = pos.X > rttfr;

    if (throughFarWall) {

        debugMap[60-22] = "Far YZ collision";
        return COLL_FAR_YZ;
    }

    debugMap[60-22] = "No YZ collision";
    return COLL_NONE_YZ;
}

int Camera::collisionYX(Coord pos) {
    if (whichPlace == IN_HALL){
        if(hallTopFarRight.Z < pos.Z) {

            debugMap[60-23] = "Hall Right YX collision";
            return COLL_HALL_RYX;
        }
        if(hallBottomNearLeft.Z > pos.Z) {

            debugMap[60-23] = "Hall Left YX collision";
            return COLL_HALL_LYX;
        }
    }if(whichPlace == IN_ROOM){

        if(roomTopFarRight.Z < pos.Z) {

            debugMap[60-23] = "Room Right YX collision";
            return COLL_ROOM_RYX;
        }
        if(roomBottomNearLeft.Z > pos.Z) {

            debugMap[60-23] = "Room Left YX collision";
            return COLL_ROOM_LYX;
        }
    }
    debugMap[60-23] = "No YX collision";
    return COLL_NONE_YX;
}

int Camera::collisionXZ(Coord pos) {
    if (pos.Y > roomTopFarRight.Y) {
        debugMap[60-24] = "Up XZ collision";
        return COLL_UP_XZ;
    }
    if (pos.Y < roomBottomNearLeft.Y) {
        debugMap[60-24] = "Down XZ collision";
        return COLL_DOWN_XZ;
    }
    debugMap[60-24] = "No XZ collision";
    return COLL_NONE_XZ;
}


void Camera::getRelTrans(Coord deltaTranslation) {
    float cosYaw = cos(ang.Y);
    float sinYaw = sin(ang.Y);

    Coord transVec;
    transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
    transVec.Y = deltaTranslation.Y;
    transVec.Z = deltaTranslation.X * sinYaw + deltaTranslation.Z * cosYaw;

    pos = pos + transVec;
    tgt = tgt + transVec;
}





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

void Camera::updateCardInfo() {
    cardDist =pos.dist(Coord(18.6000004, 1.84333336, 0.25));
    debugMap[60-25] = "Card Distance: " + std::to_string(cardDist);
}

void Camera::moveCamWithColl(Coord deltaTranslation) {
    float cosYaw = cos(ang.Y);
    float sinYaw = sin(ang.Y);

    Coord transVec;
    transVec.X = deltaTranslation.X * cosYaw - deltaTranslation.Z * sinYaw;
    transVec.Y = deltaTranslation.Y;
    transVec.Z = deltaTranslation.X * sinYaw + deltaTranslation.Z * cosYaw;
    Coord a = pos + transVec;
    if(useCollision) {
        int cYZ = collisionYZ(a);
        int cYX = collisionYX(a);
        int CXZ = collisionXZ(a);

        if( cYZ != COLL_NONE_YZ){
            transVec.X = 0;
        } if( cYX != COLL_NONE_YX){
            transVec.Z = 0;
        } if( CXZ != COLL_NONE_XZ){
            transVec.Y = 0;
        }
    }
    pos = pos + transVec;
    tgt = tgt + transVec;
    // check distance to card:

    updateCardInfo();
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


//                  __       _____                          __
//    ______________\ \     / ___/________  ____  ___  ____/ /________ __      __ _________  ____
//   / ___/ ___/ ___/\ \    \__ \/ ___/ _ \/ __ \/ _ \/ __  / ___/ __ `/ | /| / // ___/ __ \/ __ \
//  (__  ) /  / /__   \ \  ___/ / /__/  __/ / / /  __/ /_/ / /  / /_/ /| |/ |/ // /__/ /_/ / /_/ /
// /____/_/   \___/    \_\/____/\___/\___/_/ /_/\___/\__,_/_/   \__,_/ |__/|__(_)___/ .___/ .___/
//                                                                                 /_/   /_/

// #include "Scenedraw.h"
// #include "Coord.h"
// #include "ColorData.h"
// #include "globals.h"
// #include "things.h"
// #include "lighting.h"
// #include "Camera.h"


extern std::map<int, std::string> debugMap;
extern Camera cam;
extern Blinds windowBlinds;

Coord hallBnl = Coord(hallBnlF);
Coord halltfr = Coord(halltfrF);

//Coord roomBnl = Coord(halltfr.X, 0, 3*hallBnl.Z);
Coord roomBnl = Coord(halltfr.X, hallBnl.Y, 3 * hallBnl.Z);
Coord roomtfr = Coord(2 * halltfr.X, halltfr.Y, 3 * halltfr.Z);


bool roomCollides(Coord loc) {
    return !((loc.X < roomBnl.X || loc.X > roomtfr.X)
        && (loc.Y < roomBnl.Y || loc.Y > roomtfr.Y)
        && (loc.Z < roomBnl.Z || loc.Z > roomtfr.Z));

}
bool hallCollides(Coord loc) {
    return !(((loc.X < hallBnl.X || loc.X > halltfr.X)|| doorOpenPercent > 90)
        && (loc.Y < hallBnl.Y || loc.Y > halltfr.Y)
        && (loc.Z < hallBnl.Z || loc.Z > halltfr.Z));
}


Coord troubleshootingBnl = halltfr * 0.45;
Coord troubleshootingtfr = halltfr * 0.55;
Coord lampPos = Coord(roomBnl.X+5, halltfr.Y-0.3, 0);

float t() {
    return pow(1.15, (-1.3 * (cam.pos.dist(lampPos) - 12))) + 1;
}

#include <vector>
#include <cmath>

std::vector<float> returnEvenlySpaced(int requestedPoints, float minDist, float maxVal) {
    if (requestedPoints <= 0 || minDist <= 0 || maxVal < 0) {
        return {};
    }
    float idealSpacing = (maxVal - minDist) / (requestedPoints - 1);
    if (idealSpacing >= minDist) {
        std::vector<float> points(requestedPoints);
        for (int i = 0; i < requestedPoints; ++i) {
            points[i] = minDist * i + minDist;
        }
        return points;
    } else {
        // Minimum distance cannot be satisfied, calculate how many points fit
        int actualPoints = std::floor((maxVal - minDist) / minDist) + 1;

        // Create the vector with adjusted spacing (maxVal divided by actual points)
        std::vector<float> points(actualPoints);
        float step = maxVal / (actualPoints - 1);
        for (int i = 0; i < actualPoints; ++i) {
            points[i] = i * step;
        }
        return points;
    }
}


Coord tableBnl = Coord(roomtfr.X - 2, roomBnl.Y, -1.5);
Coord tableTfr = Coord(roomtfr.X - 1, roomtfr.Y / 3, 1.5);
float legInset = 0.2;
float tableThickness = 0.1;
float legRadT = 0.1;
float legRadB = 0.1;

float doorwayHeight = 3;
float doorwayWidth = 2;

Coord blindsBnl = Coord((roomtfr.X-roomBnl.X)/2+roomBnl.X, 1, roomBnl.Z);
Coord blindsTfr = Coord((roomtfr.X-roomBnl.X)/2+roomBnl.X, roomtfr.Y-1, roomBnl.Z);

void drawBlinds(Coord bnl, Coord tfr) {
    glPushMatrix();
    glTranslatef(bnl.X, bnl.Y, bnl.Z);
    glRotatef(-90, 0, 1, 0);
    windowBlinds.draw(ALL);
    glPopMatrix();
}


void drawCardWinner1(int winnernum = 0) {
    // Set the color to yellow
    glShadeModel(GL_FLAT);
    cardMat.apply();
    glColor3f(0, 1.0f, 1.0f);
    // Draw the flattened dodecahedron
    glPushMatrix();
    glScalef(1.0f, 1.0f, 0.01f); // Scale in Y dimension to flatten
    glutSolidDodecahedron();
    glPopMatrix();
    glShadeModel(GL_SMOOTH);
    // Set the normal in the Y dimension
    // glNormal3f(0.0f, 1.0f, 0.0f);


    lampMat.apply();
    // Draw stroke text
    glLineWidth(5);
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-1.3f, -0.01f, 0.1f); // Position the text
    glScalef(0.0045f, 0.0045f, 0.0045f); // Scale the text

    std::string texts = retWinner();
    const char *text = texts.c_str();

    debugMap[60] = "             winner:" + std::string(texts);
    debugMap[61] = "             winner:" + std::string(text);
    while (*text) {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *text++);
    }
    glPopMatrix();
}


void makeLeg(float *legInfo) {
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    GLUquadricObj *quadric = gluNewQuadric();
    gluCylinder(quadric, legInfo[2], legInfo[0], legInfo[1], 32, 32);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

void drawLeg(Coord legLocBottom, float legInfo[3]) {
    glPushMatrix();
    glTranslatefv(legLocBottom + Coord(0, legInfo[1], 0));
    castIronMat.apply();
    // make vertical cylinders
    makeLeg(legInfo);
    glPopMatrix();
}

void drawTable(Coord bnl, Coord tfr, float lInset = 0.2, float tThick = 0.1, float legRad = 0.1) {
    //table top
    tableMat.apply();
    cubeOfPlanes(Coord(bnl.X, tfr.Y - tThick, bnl.Z), tfr, 40, 1, ALL_FACE);



    /*     legs, use glutcylinders
     *     leg order:
     *      1--2
     *      |  |b
     *      0--3    */


    Coord leg0Loc = bnl + Coord(legInset, 0, legInset);
    Coord leg1Loc = Coord(tfr.X - legInset, 0, bnl.Z + legInset);
    // Coord leg1Loc = bnl + Coord(tfr.X-bnl.X - legInset, 0, legInset);
    Coord leg2Loc = tfr- Coord(legInset, tfr.Y, legInset);
    Coord leg3Loc = Coord(bnl.X +legInset, 0, tfr.Z - legInset);
    Coord legInfo = Coord(legRadT/4, tfr.Y - tThick - bnl.Y, legRadB/2);

    drawLeg(leg0Loc, legInfo);
    drawLeg(leg1Loc, legInfo);
    drawLeg(leg2Loc, legInfo);
    drawLeg(leg3Loc, legInfo);
}

void drawContainer(Coord bnl, Coord tfr) {
    //simple cube, no top
    //drawing it twice to get the inside and outside
    glPushMatrix();
    shinyRed.apply();
    cubeOfPlanes(bnl, tfr, 20, OUTSIDEOUT, ALL_FACE ^ TOP_FACE);
    cubeOfPlanes(bnl+0.005, tfr-0.005, 20, INSIDEOUT, ALL_FACE ^ TOP_FACE);
    glPopMatrix();
}

 void lDoorExist() {
    //door: simple cube
    superShinySteel.apply();
    float animPercent = -2.0f*(doorOpenPercent / 100.0);
    glPushMatrix();
    glTranslatef(0, 0, animPercent);
    cubeOfPlanes(Coord(halltfr.X - 0.4, hallBnl.Y, hallBnl.Z),
                 Coord(halltfr.X - 0.1, hallBnl.Y + doorwayHeight, 0), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();
}

void rDoorExist() {
    //just reflect it over the Z, the normals will be "good enough"
    superShinySteel.apply();
    glPushMatrix();
    glScalef(1, 1, -1); //reflect over Z
    lDoorExist();
    glPopMatrix();
}

void drawLampCage() {
    //enable smooth lines, hopefully I'll be able to get the look I'm going for
    glEnable(GL_LINE_SMOOTH);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glutWireSphere(0.32, 8, 5);
    glPopMatrix();
    glDisable(GL_LINE_SMOOTH);
}

void drawLamp() {
    lampMat.apply();
    glPushMatrix();
    glutSolidSphere(0.3, 20, 20);
    glPopMatrix();

    castIronMat.apply();
    glLineWidth(t());

    glPushMatrix();
    drawLampCage();
    glPopMatrix();
    glLineWidth(1);
}

void lockInCardNormals() {
    glPushMatrix();
    glNormal3f(0, 0, 1);
    drawCardWinner1();
    glPopMatrix();
}

void drawCardFinalPos() {
    glPushMatrix();
    glRotatef(270, 0, 1, 0);
    glRotatef(270, 1, 0, 0);
    lockInCardNormals();
    glPopMatrix();
}

void drawWinnerAndRotate() {
    Coord finalRot = Coord(3*360, 7*360, 90);
    float dumb = cardRotPercent;
    float cardRot = dumb/(cardRotSpeed*100);
    Coord currentRot = finalRot * cardRot;

    glPushMatrix();
    glScalef(0.9f*cardRot+0.1f, 0.9f*cardRot+0.1f, 0.9f*cardRot+0.1f);
    glTranslatef(0, 3*cardRot, 0);
    glRotatef(currentRot.X, 1, 0, 0);
    glRotatef(currentRot.Y, 0, 1, 0);
    glRotatef(currentRot.Z, 0, 0, 1);
    drawCardFinalPos();
    glPopMatrix();
}

void drawHall() {
    sunLight.disable();
    ColorData lightOG[3] = {
        hallLight.lightAmb,
        hallLight.lightDiff,
        hallLight.lightSpec
    };
    glEnable(GL_LIGHTING);
    hallLight.lightAmb = roomLight.lightAmb * doorOpenPercent / 110;
    hallLight.lightDiff = roomLight.lightDiff * doorOpenPercent / 110;
    hallLight.lightSpec = roomLight.lightSpec * doorOpenPercent / 110;
    hallLight.setup();
    hallLight.enable();
    hallLight.lightAmb = lightOG[0];
    hallLight.lightDiff = lightOG[1];
    hallLight.lightSpec = lightOG[2];



    headLamp.enable();

    //get rid of this later:
    // glPushMatrix();
    // glTranslatefv(lampPos);
    // drawLamp();
    // glPopMatrix();
    // roomLight.enable();
    int siding = OUTSIDEOUT;
#ifndef FOLDING_REGION_HALLWAY

    wallMat.apply();
    //doorframe, but only the back face:
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y, hallBnl.Z),
                 Coord(halltfr.X, hallBnl.Y + doorwayHeight, hallBnl.Z + doorwayWidth / 2), 20, OUTSIDEOUT, BACK_FACE);
    //top of doorframe
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y + doorwayHeight, hallBnl.Z),
                 Coord(halltfr.X, halltfr.Y, halltfr.Z), 20, OUTSIDEOUT, BACK_FACE);

    //Left side of doorframe
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y, halltfr.Z - doorwayWidth / 2),
                 Coord(halltfr.X, hallBnl.Y + doorwayHeight, halltfr.Z), 20, OUTSIDEOUT, BACK_FACE);


    //
    cubeOfPlanes(hallBnl, Coord(roomtfr.X, halltfr.Y, halltfr.Z), 40, INSIDEOUT, BACK_FACE);
    cubeOfPlanes(hallBnl, halltfr, 40, INSIDEOUT, LEFT_FACE | RIGHT_FACE);
    floorMat.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, INSIDEOUT, BOTTOM_FACE);
    ceilingMat.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, INSIDEOUT, TOP_FACE);
#endif


#ifndef FOLDING_REGION_ROOM

    hallLight.disable();
    glPushMatrix();
    glTranslatefv(lampPos);
    drawLamp();
    glPopMatrix();
    roomLight.setup();
    roomLight.enable();

    wallMat.apply();
    cubeOfPlanes(roomBnl, Coord(roomtfr.X, roomtfr.Y, hallBnl.Z), 40, INSIDEOUT, FRONT_FACE | BACK_FACE);
    matteConcrete.apply();
    cubeOfPlanes(Coord(halltfr.X, roomBnl.Y, halltfr.Z), roomtfr, 40, INSIDEOUT, FRONT_FACE | BACK_FACE);


    drawBlinds(blindsBnl, blindsTfr);
    glPushMatrix();
    glScalef(1, 1, -1);
    drawBlinds(blindsBnl, blindsTfr);
    glPopMatrix();
    Coord bl1 = blindsBnl-Coord(1, 0, 0);
    Coord bl2 = {bl1.X, blindsTfr.Y, bl1.Z};
    Coord bl3 = blindsTfr;
    Coord bl4 = (bl1&0b011)|Coord(blindsTfr.X+1, 0, 0);

    Coord br1 = Coord(bl1.X, bl1.Y, -bl1.Z);
    Coord br2 = Coord(bl2.X, bl2.Y, -bl2.Z);
    Coord br3 = Coord(bl3.X, bl3.Y, -bl3.Z);
    Coord br4 = Coord(bl4.X, bl4.Y, -bl4.Z);


    wallMat.apply();
    cubeOfPlanes(roomBnl, br4, 20, INSIDEOUT, LEFT_FACE|RIGHT_FACE);
    cubeOfPlanes(Coord(roomBnl.X, bl1.Y, bl1.Z), {br1.X, roomtfr.Y, roomtfr.Z}, 20, INSIDEOUT, LEFT_FACE|RIGHT_FACE);
    cubeOfPlanes(bl2, roomtfr, 20, INSIDEOUT, LEFT_FACE|RIGHT_FACE);
    cubeOfPlanes(bl4-Coord(0, 1, 0), (roomtfr&101)|br3, 20, INSIDEOUT, LEFT_FACE|RIGHT_FACE);

    floorMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, INSIDEOUT, BOTTOM_FACE);
    ceilingMat.apply();
    cubeOfPlanes(roomBnl, roomtfr, 40, INSIDEOUT, TOP_FACE);


    //doorframe, but not the back face:
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y, hallBnl.Z),
                 Coord(halltfr.X, hallBnl.Y + doorwayHeight, hallBnl.Z + doorwayWidth / 2),
                 20, 1,ALL_FACE ^ BACK_FACE);

    //top of doorframe
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y + doorwayHeight, hallBnl.Z),
                 Coord(halltfr.X, halltfr.Y, halltfr.Z), 20, OUTSIDEOUT, ALL_FACE ^ BACK_FACE);

    //Left side of doorframe
    cubeOfPlanes(Coord(halltfr.X - 0.5, hallBnl.Y, halltfr.Z - doorwayWidth / 2),
                 Coord(halltfr.X, hallBnl.Y + doorwayHeight, halltfr.Z), 20, OUTSIDEOUT, ALL_FACE ^ BACK_FACE);


    //this one does the gap in the back of the room,
    wallMat.apply();
    cubeOfPlanes(hallBnl, Coord(roomtfr.X, halltfr.Y, halltfr.Z), 40, INSIDEOUT, FRONT_FACE);


    //doors:
    lDoorExist();
    rDoorExist();

    //drawing the table
    drawTable(tableBnl, tableTfr);
#endif
    glPushMatrix();
    glTranslatef(0.1 + tableBnl.X, tableTfr.Y, -0.25 );
    drawContainer(Coord(0,0,0), Coord(0.5,0.51,0.5));
    glPopMatrix();

    float teapotSize = 0.3;
    matteConcrete.apply();
    glPushMatrix();
    //draw teapot on table:
    glTranslatef(0.1 + tableBnl.X+0.25, tableTfr.Y+0.5*teapotSize, 0.7 );
    glRotatef(115, 0, 1, 0);
    glutSolidTeapot(teapotSize);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.4 + tableBnl.X, tableTfr.Y, 0 );
    glScalef(0.25, 0.25, 0.25);
    drawWinnerAndRotate();
    glPopMatrix();


#ifndef FOLDING_REGION_DEBUGCUBES


    glPushMatrix();
    glTranslatef(10, 10, 10);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, 1, FRONT_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, 1, TOP_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, 1, RIGHT_FACE);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, 1, BACK_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, 1, BOTTOM_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, 1, LEFT_FACE);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(7, 10, 10);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, -1, FRONT_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, -1, TOP_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, -1, RIGHT_FACE);
    shinyRed.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, -1, BACK_FACE);
    shinyGreen.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, -1, BOTTOM_FACE);
    shinyBlue.apply();
    cubeOfPlanes(Coord(), Coord() + 2.0f, 100, -1, LEFT_FACE);
#endif


    glPopMatrix();
            // glEnable(GL_LIGHTING);


    roomLight.disable();
    sunLight.enable();
}

void lastHiddenCubeMirrored() {
    glColor3f(0,1,0);
    glPushMatrix();
    glTranslatef(15,-5.5 , 2);
    cubeOfPlanes(Coord(), Coord(1, -3, 1), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();
}
void drawHiddenBuffer() {
    glDisable(GL_LIGHTING);

    //blue cube:
    glColor3f(0, 0, 255);
    glPushMatrix();
    glTranslatef(9, 1, 1);
    cubeOfPlanes(Coord(), Coord(1, 1, 1), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

    //red; red box
    glColor3f(255, 0, 0);
    glPushMatrix();
    glTranslatef(17.6, 1, -0.5f);
    cubeOfPlanes(Coord(), Coord(1, 1, 1), 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

    //green cube:
    glColor3f(0, 255, 0);
    glPushMatrix();
    cubeOfPlanes(blindsBnl-Coord(1,0,0), {blindsTfr.X+1, blindsTfr.Y, blindsBnl.Z+0.1f}, 20, OUTSIDEOUT, ALL_FACE);
    cubeOfPlanes(blindsBnl+Coord(-1,0,11.9), {blindsTfr.X+1, blindsTfr.Y, 12}, 20, OUTSIDEOUT, ALL_FACE);
    glPopMatrix();

    //magenta cube:
    glColor3f(255, 0, 255);
    glPushMatrix();
    // cubeOfPlanes(hallBnl + Coord(-1.25, 1.5f, 3.5f),hallBnl + Coord(-0.25, 2.5f, 4.5f), 10, OUTSIDEOUT, ALL_FACE);
    glTranslatef(10, 2, -3);
    glutSolidCube(1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}
void drawClickableObjects() {
    //doorSwitch:
    castIronMat.apply();
    glPushMatrix();
    glTranslatef(9.5, 1.5, 1.5);

    glScalef(0.5, 0.3, 0.3);
    glutSolidIcosahedron();
    glPopMatrix();

    //lightSwitch:

    glPushMatrix();
    glScalef(0.25, 0.25, 0.25);
    glTranslatefv(hallBnl + Coord(-1.25, 1.5f, 3.5f)+ 0.5);
    testConeA(1, ColorData(0.1f, 0.1f, 0.1f, 1.0f));
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10, 2, -3);
    glScalef(-1, -1, -1);
    testConeCrot(Coord(D(cam.ang.X), D(cam.ang.Y-PI/2), 0),
        ColorData(0.0f, 1.0f, 0.0f, 0.3f));
    glPopMatrix();

}


//                     _
//    ____ ___  ____ _(_)___   _________  ____
//   / __ `__ \/ __ `/ / __ \ / ___/ __ \/ __ \
//  / / / / / / /_/ / / / / // /__/ /_/ / /_/ /
// /_/ /_/ /_/\__,_/_/_/ /_(_)___/ .___/ .___/
//                              /_/   /_/

#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include "src/globals.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <functional>
#include <map>
#include <fstream>

#include "src/Coord.h"
#include "src/Camera.h"
#include "src/things.h"
#include "src/ColorData.h"

#include "src/lighting.h"

#include "src/LeftVP.h"
#include "src/Scenedraw.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#ifndef FOLDING_REGION_Vport
int totalWidth = 1900;
int lVportW = 300;
int rVportW = 1600;
int height = 900;


#endif

#ifndef FOLDING_REGION_Global_Objects
Camera cam = Camera();
std::vector<Debug3Dx> debugXes;
Blinds windowBlinds;


#endif

#ifndef FOLDING_REGION_Global_Variables
float fov = 90.0;
float animThing = 0;
//Global Variables
bool useMouse = false;
bool started = false;
bool starteda = false;


float sensitivity = 0.01f; // camera movement and mouse sensitivity
float blindAnimSpeed = 0.05;
bool showInfoViewport = true;
bool drawDebugPoints = false;
bool focusPoint = false;
bool drawAllPoints = false;
int currPointDraw = 0;
//function pointer to infoVP addDebugString

std::map<int, std::string> debugMap;

int alt, ctrl, modifiers, shift;
bool showKeybinds = true;
std::vector<std::string> instructionVec = {
    "======Keybinds======",
    "F1: Toggle Information Panel",
    "F2: Shrink Console",
    "F3: Grow Console",
    "F5: Load Camera States from disk",
    "F9: Save Camera states to disk",
    "F12: Print test bitmap Alphabet",
    "1-5: Load Camera state from memory slot",
    "!-%: Save Camera state to memory slot",
    "PGUP, PGDN: Pitch camera",
    "HOME: FOV++",
    "END: FOV --",
    "Up/DArrow: Move forward/back in the XZ plane",
    "L/RArrow: Rotate Clockwise/Counterclockwise about Y",
    "W/D: Move forward/back relative to camera in XZ",
    "A/S: Strafe Left/Right in XZ",
    "F/C: Ascend/Descend",
    "SPACE: Toggle mouse Camera Control",
    "?: View/Hide Keybindings (return to console)"
};

//mid-dark grey, kinda like blender's default background
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};
ColorData rVPColorData = ColorData(0.2, 0.2, 0.2, 1.0);
ColorData solarizedBG = ColorData(0.02745, 0.21176, 0.25882, 1.0);
ColorData solarizedText = ColorData(0.71373, 0.58039, 0.58824, 1.0);

DebugLevel defaultDebug = WEAK;
bool detachSpotlight = false;
//variables for FPS Counter:


#endif

#ifndef FOLDING_REGION_MATERIALS

//materials and lights
// Spotlight headlamp;
Light fakeSun;

int frame = 0;
auto prevTime = std::chrono::high_resolution_clock::now();

void calculateFPS() {
    frame++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - prevTime);
    if (duration.count() >= 1) {
        debugMap[60 - 3] = " FPS: " + std::to_string(frame);
        frame = 0;
        prevTime = currentTime;
    }
}
#endif

#ifndef FOLDING_REGION_Draw


Coord rotToVec(float rotRadX, float rotRadY) {
    return Coord(cos(rotRadX) * sin(rotRadY), sin(rotRadX), cos(rotRadX) * cos(rotRadY));
}

Coord normalize(Coord vec) {
    float sqr = vec.X * vec.X + vec.Y * vec.Y + vec.Z * vec.Z;
    float invSqr = 1 / sqrt(sqr);
    return Coord(vec.X * invSqr, vec.Y * invSqr, vec.Z * invSqr);
}

void updateSpotlight() {
    headLamp.setup();
    headLamp.lightPos = ColorData(0, 0, 0, 1.0f);
    headLamp.spotDir = Coord(0, 0, -1);
}

void setupRight() {
    int effectiveWidth = showInfoViewport ? totalWidth - lVportW : totalWidth;

    if (showInfoViewport) {
        // glViewport(lVportW, 0, rVportW, height);
        glViewport(lVportW, 0, effectiveWidth, height);
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, totalWidth, height);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov, static_cast<float>(effectiveWidth) / height, 0.0001, 500.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    if (cardRotState == CARD_ROT_REL) {
        cardRotPercent = std::clamp((100.0f - ((cardDist - 2.0f) / 8.0f * 100.0f)) * cardRotSpeed, 0.0f, 100.0f);

        //cardRotPercent = std::clamp(cardRotSpeed * ((cardDist + 1.0f) / 8.0f) * 100.0f, 0.0f, 100.0f);

        //rdRotPercent = (std::clamp((((cardDist+1)9)*100.0f), 0.0f, 100.0f));
    }
    updateSpotlight();
    headLamp.setup();

    cam.lookAt();
    //enable global ambient and set it to  globAmb:

    // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}

//update spotlight position and direction to match the camera
void backToBasicsCalculateTheDirVec(Coord directionVector) {
}

int modifier;

void drawLitShapes() {

    glEnable(GL_LIGHTING);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    sunLight.setup();
    headLamp.enable();

    drawHall();
    drawClickableObjects();

    sunLight.enable();

    shinyRed.apply();
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(25, 50, 150, 100);
    glPopMatrix();
    shinyGreen.apply();
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(25, 50, 250, 100);
    glPopMatrix();
    shinyBlue.apply();
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glutSolidTorus(25, 50, 250, 100);
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(headLamp);
}

void drawUnlitShapes() {
    glDisable(GL_LIGHTING);
    if (defaultDebug != NONE) {
        glPushMatrix();
        drawXZxGridlines(50);
        glPopMatrix();
        glPushMatrix();
        for (Debug3Dx debug_x: debugXes) {
            debug_x.draw();
        }

        glPopMatrix();
    }

    if (bufferPeeking) {
        drawHiddenBuffer();
    }

    if (drawAllPoints) {
        std::vector<thingHolder> debugPoints = getDbgPts();
        for (float *point: debugPoints) {
            Debug3Dx(point).draw();
        }
    }

    if (drawDebugPoints) {
        float *dbg_pts = getDbgPts(currPointDraw);
        Debug3Dx(dbg_pts).draw();
        if (focusPoint) {
            cam.tgt = Coord(dbg_pts[0], dbg_pts[1], dbg_pts[2]);
        }
    }


    // testConeArot();


    glColor3f(1, 0, 1);
    glPushMatrix();
    glutSolidCone(1, 1, 20, 20);
    glPopMatrix();

    // testCamBindings();

    // testInRightPlace(cam);


    testDrawingCubes();

    windowTest();
    glEnable(GL_LIGHTING);
}

int stateSwitch(int animState) {
    switch (animState) {
        case DOOR_CLOSED_STOPPED:
            return DOOR_OPENING;
            break;
        case DOOR_OPENED_STOPPED:
            return DOOR_CLOSING;
            break;
        case DOOR_OPENING:
            return DOOR_OPENED_STOPPED;
            break;
        case DOOR_CLOSING:
            return DOOR_CLOSED_STOPPED;
            break;
        default:
            break;
    }
}

void activateDoor() {
    animateDoor = stateSwitch(animateDoor);
}

void activateWindow() {
    animatingBlinds = stateSwitch(animatingBlinds);
}

void mainLightAction() {
    std::string hallLightState;
    roomLight.lightswitch();
    hallLight.lightswitch();
    hallLightState = roomLight.enabled ? "On" : "Off";
    glout << "Room Light switched " << hallLightState << std::endl;
}

void getID(int x, int y) {
    unsigned char pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    //printed only for demonstration
    // glout << "Pixel at (" << x << ", " << y << "): R: "
    //         << (int) pixel[0] << " G: " << (int) pixel[1] << " B: " << (int) pixel[2] << std::endl;
    //
    int pixeli[3] = {(int) pixel[0], (int) pixel[1], (int) pixel[2]};
    int pixela = pixeli[0] << 16 | pixeli[1] << 8 | pixeli[2];
    switch (pixela) {
        case (255 << 16) | (0 << 8) | 0:
            if (cardRotState == CARD_ROT_COMPLETE) {
                cardRotState = CARD_ROT_UNDO;
            } else {
                cardRotState = CARD_ROT_NOW;
            }
            break;
        case(0 << 16) | (255 << 8) | 0:
            activateWindow();
            break;
        case (0 << 16) | (0 << 8) | 255:
            activateDoor();
            break;
        case (255 << 16) | (0 << 8) | 255:
            mainLightAction();
        break;
        default:
            return;
    }
}

void drawWindow() {
    if (showInfoViewport) {
        setupLeft();
        calculateFPS();
        drawLeft();
    }
    setupRight();
    if (selecting) {
        //
        drawHiddenBuffer();
        if (selecting != selectLock) {
            getID(xClick, yClick);
        }
        selecting = false;
        glutSwapBuffers();
        if (selectLock) {
            selecting = true;
            return;
        }
    }
    drawUnlitShapes();

    // drawMoreShapes();

    headLamp.enable();

    drawLitShapes();


    glutSwapBuffers();
}
#endif

#ifndef FOLDING_REGION_UTILITY_AND_SETUP

bool shownKeybinds = false;
// fps function

void showKeybindings() {
    glout << NOPREFIX;
    glout << CLEARALL;
    glout << MAX;
    for (const std::string &i: instructionVec) {
        glout << i << '\n';
    }
}

void myPause(int pauseLength = 100) {
    int tStart = clock();
    while (clock() - tStart < pauseLength) { ; }
}

void setupObjects() {
    Coord camIniPos(1, 2, 0);
    Coord camIniTgt(2, 2, 0);
    cam = Camera(camIniPos,camIniTgt, Coord(0, 1, 0));
    cam.relRot({1, 1,0});
    cam.relRot({-1, -1,0});//prevents the camera from jumping at the start
    // cam.relRot(Coord(0, 90, 0));
    // Coord pitchYaw= calcPitchYaw(camIniPos, camIniTgt);
    // cam.setPitchYaw();
    // cam.tgt = camIniTgt;
    debugXes.emplace_back(Coord(0, 0, 0), 100, 2);

    windowBlinds = Blinds(2, 2, 0.1, 30);


    // roomLight = Light(x

    headLamp.enable();

    // headLamp.setup();

    //giving them access to the debugging info map
    cam.setDebugStringAdd(&debugMap);
    windowBlinds.setDebugStringAdd(&debugMap);

    //setup lvp class:
}

void setupLights() {
    float lightAmb[] = {0.8, 0.7, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {0.8, 0.7, 0.2, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 7.0, 0.0, 0.0}; // Position remains the same
    // Cool global ambient light

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Spotlight headlamp = Spotlight(Light::lightNum(GL_LIGHT2), ColorData(cam.pos, 0.0f), ColorData(0.2, 0.2, 0.2, 1.0), ColorData(1.0, 1.0, 1.0, 1.0),
    //                                ColorData(1.0, 1.0, 1.0, 1.0), cam.tgt, 30.0, 1.0);

    //enabling global ambient light:
    //
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    //setup the sun using a positional light:
    sunLight.setup();

    headLamp.enable();
}

void setup() {
    winner = useTimeToSeedRandomToSetWinner();
    roomBnlF[0] = 2 * hallBnlF[0];
    roomBnlF[1] = hallBnlF[1];
    roomBnlF[2] = 3.0f * hallBnlF[2];

    roomtfrF[0] = 2 * halltfrF[0];
    roomtfrF[1] = halltfrF[1];
    roomtfrF[2] = 3 * halltfrF[2];
    // Light property vectors.


    float lightAmb[] = {0.8, 0.7, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {0.8, 0.7, 0.2, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 7.0, 0.0, 1.0}; // Position remains the same;

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Spotlight headlamp = Spotlight(Light::lightNum(GL_LIGHT2), ColorData(cam.pos, 0.0f), ColorData(0.2, 0.2, 0.2, 1.0), ColorData(1.0, 1.0, 1.0, 1.0),
    //                                ColorData(1.0, 1.0, 1.0, 1.0), cam.tgt, 30.0, 1.0);

    //enabling global ambient light:
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.


    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE); // Enable automatic normalization of normals.
    glShadeModel(GL_SMOOTH);
    //check if light0 is enabled with a printStatement:
    //glListallEnabled:

    setupObjects();
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}

void resize(int w, int h) {
    totalWidth = w;
    height = h;

    lVportW = 0.2 * totalWidth; // 20% of total width
    rVportW = totalWidth - lVportW;
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (float) w / (float) h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}
#endif

#ifndef FOLDING_REGION_Control

void mouseControl(int button, int state, int x, int y) {
    //check to see if the window is focused
    if (state == GLUT_DOWN && button == GLUT_LEFT) {
        selecting = true;
        selectLock = selectLockingEnabled;
        // if selectLock, set
        glutSetCursor(selectLock ? GLUT_CURSOR_LEFT_ARROW : GLUT_CURSOR_FULL_CROSSHAIR);
        if (!selectLock) {
            xClick = x;
            yClick = height - y;
        } //for screen vs mouse coordinates}}
        glutPostRedisplay();
    }
    if (state == GLUT_UP && button == GLUT_LEFT) {
        selectLock = false;
        //reset cursor to default:
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        xClick = x;
        yClick = height - y;
    }
}

static bool firstMouse = true;

void mouse(int x, int y) {
    static int lastX = 0, lastY = 0;

    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    int deltaX = x - lastX;
    int deltaY = lastY - y;
    lastX = x;
    lastY = y;

    cam.relRot(Coord(deltaY * sensitivity, deltaX * sensitivity, 0));

    glutPostRedisplay();
}

void toggleMouse() {
    useMouse = !useMouse;
    if (useMouse) {
        glutPassiveMotionFunc(mouse);
    } else {
        glutPassiveMotionFunc(NULL);
    }
}

void modGlobAmb(int modifier) {
    globAmb[0] = clmp(globAmb[0] + modifier, 0.0, 1.0);
    globAmb[1] = clmp(globAmb[1] + modifier, 0.0, 1.0);
    globAmb[2] = clmp(globAmb[2] + modifier, 0.0, 1.0);
}

void keyboard(unsigned char key, int x, int y) {
    modifiers = glutGetModifiers();
    std::string hallLightState;

    switch (key) {
        case 'i': //CAMERA FORWARD
            cam.moveCamWithColl(Coord(1 * moveSpeed, 0, 0));
            break;
        case 'k': //CAMERA BACKWARD
            cam.moveCamWithColl(Coord(-1 * moveSpeed, 0, 0));
            break;
        case 'j': //CAMERA LEFT
            cam.moveCamWithColl(Coord(0, 0, -1 * moveSpeed));
            break;
        case 'l': //CAMERA RIGHT
            cam.moveCamWithColl(Coord(0, 0, 1 * moveSpeed));
            break;

        case 'D':
                mainLightAction();
            break;
        case ';': //CAMERA DOWN
            cam.moveCamWithColl(Coord(0, -1 * moveSpeed, 0));
            break;
        case '.': //CAMERA UP
            cam.moveCamWithColl(Coord(0, 1 * moveSpeed, 0));
            break;
        case 'd':
            if (modifiers & GLUT_ACTIVE_ALT) {
                activateDoor();
            } else {
                mainLightAction();
                activateDoor();
            }
            break;
        case ' ': //Toggle Mouse control of Camera
            if (modifiers & GLUT_ACTIVE_SHIFT) {
                useCollision = !useCollision;
                glout << "Collision: " << (useCollision ? "Enabled" : "Disabled") << '\n';
            } else {
                toggleMouse();
                glout << "Mouse Control: " << (useMouse ? "On" : "Off; Use <->  and PGUP/DN") << '\n';
            }
            break;
        case 'w':
            activateWindow();
            break;
        case '1':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ FRONT_FACE;
                glout << "FRONT_FACE: " << (enabledFaces & FRONT_FACE ? "Enabled" : "Disabled") << '\n';
            } else {
                cam.restoreState(0);
                glout << "Camera State 1 Restored" << '\n';
                glout << "Pos: " << cam.pos.toString(0) << "Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '2':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ TOP_FACE;
                glout << "TOP_FACE: " << (enabledFaces & TOP_FACE ? "Enabled" : "Disabled") << '\n';
            } else {
                cam.restoreState(1);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos: " << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '3':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ RIGHT_FACE;
                glout << "RIGHT_FACE: " << (enabledFaces & RIGHT_FACE ? "Enabled" : "Disabled") << std::endl;
            } else {
                cam.restoreState(2);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '4':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ BACK_FACE;
                glout << "BACK_FACE : " << (enabledFaces & BACK_FACE ? "Enabled" : "Disabled") << std::endl;
            } else {
                cam.restoreState(3);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '5':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ BOTTOM_FACE;
                glout << "BOTTOM_FACE : " << (enabledFaces & BOTTOM_FACE ? "Enabled" : "Disabled") << std::endl;
            } else {
                cam.restoreState(4);
                glout << "Camera State " << key << " Restored" << '\n';
                glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            }
            break;
        case '6':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces ^ LEFT_FACE;
                glout << "LEFT_FACE Face: " << (enabledFaces & LEFT_FACE ? "Enabled" : "Disabled") << std::endl;
            }
        case '!': cam.storeState(0);
            glout << "State1:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '@': cam.storeState(1);
            glout << "State2:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '#': cam.storeState(2);
            glout << "State3:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '$': cam.storeState(3);
            glout << "State4:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '%': cam.storeState(4);
            glout << "State5:" << "Pos:" << cam.pos.toString(0) << " Cam Tgt " << cam.tgt.toString(0) << '\n';
            break;
        case '`':
            if (modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces | ALL_FACE;
                glout << "All Faces Enabled" << '\n';
            } else {
                dbgNormals = nextDbgState();
                glout << "NormalColorization: " << dbgNormMap[dbgNormals] << '\n';
            }
            break;
        case 'G':

            modGlobAmb(0.01);
            glout << "Global Ambient Light: " << globAmb[0] << ", " << globAmb[1] << ", " << globAmb[2] << '\n';
            break;
        case 'g':
            modGlobAmb(-0.01);
            glout << "Global Ambient Light: " << globAmb[0] << ", " << globAmb[1] << ", " << globAmb[2] << '\n';


            break;
        case 'h':
            headLamp.lightswitch();
            break;
        case '?': //print keybinds:
            showKeybindings();
            break;

        case 27: //Escape Key: Exit
        default:
            break;
    }
    glutPostRedisplay();
}

int counter = 0;

void testCharacterPrinting() {
    for (unsigned int i = counter * 5; i <= 255; i++) {
        glout << std::hex << std::setw(2) << std::setfill('0') << i << ": ";
        glout << (char) i << " ";
        if (i % 5 == 4) {
            glout << '\n';
            counter++;
            break;
        }
        if (counter >= 0xFE || i >= 0xFE)
            counter = 0;
    }
}


Coord angle = Coord(0, 0.0872665, 0); //5 degrees


void specialKeyboard(int key, int x, int y) {
    //modifiers:
    modifiers = glutGetModifiers();
    alt = modifiers & GLUT_ACTIVE_ALT;
    shift = modifiers & GLUT_ACTIVE_SHIFT;

    switch (key) {
        case GLUT_KEY_F1:
            showInfoViewport = !showInfoViewport;
            resize(totalWidth, height);
            break;
        case GLUT_KEY_F2:
            conHeightPercent = clmp(conHeightPercent - 0.02, 0.0, 0.52);
            glout << "Shrank console to " << conHeightPercent << '\n';
            break;

        case GLUT_KEY_F3:
            conHeightPercent = clmp(conHeightPercent + 0.02, 0.0, 0.52);
            glout << "Grew console to " << conHeightPercent << '\n';
            break;
        case GLUT_KEY_F4:
            headLamp.lightswitch();
            debugMap[60 - 20] = "Headlamp: " + headLamp.enabled ? "On" : "Off";

            break;
        case GLUT_KEY_F5:
            cam.loadFromFile(cameraSaveFile);

            glout << "Camera Profiles loaded from " << cameraSaveFile << '\n';
            glout << "Available States: " << '\n';
            for (int i = 0; i < 5; i++) {
                glout << i + 1 << ": " << "Pos:" << std::get<0>(cam.storedStates[i]).toString(0) <<
                        " Tgt:" << std::get<1>(cam.storedStates[i]).toString(0) << '\n';
            }
            break;

        case GLUT_KEY_F6:
            activateDoor();
            break;
        case GLUT_KEY_F7:
            //if shift
            if (modifiers & GLUT_ACTIVE_SHIFT) {
                winner = 1;
                glout << "Win-Cheat: " << retWinner() << "\n";
            } else if (modifiers & GLUT_ACTIVE_ALT) {
                winner = 0;
                glout << "Win-Cheat: " << retWinner() << "\n";;
            } else {
                winner = useTimeToSeedRandomToSetWinner();
                glout << "Win-Rand: " << retWinner() << "\n";;
            }
            break;

        case GLUT_KEY_F8:
            if (modifiers & GLUT_ACTIVE_ALT) {
                cardRotState = cardRotState == CARD_ROT_REL ? CARD_ROT_UNDO : CARD_ROT_REL;
                glout << "Interactive Card: " << (cardRotState == CARD_ROT_REL ? "Enabled" : "Disabled") << '\n';
            } else {
                if (cardRotState == CARD_ROT_NONE) {
                    cardRotState = CARD_ROT_NOW;
                } else if (cardRotState == CARD_ROT_NOW || CARD_ROT_COMPLETE) {
                    cardRotState = CARD_ROT_UNDO;
                    glout << "CardUndo" << std::endl;
                }
            }
            break;
        case GLUT_KEY_F9: //call Camera::saveToFile(std::ofstream& file)
            //open file pointer for writing:
            cam.saveToFile(cameraSaveFile);
            glout << "Camera states saved to " << cameraSaveFile << '\n';
            break;

        case GLUT_KEY_F12: //call Camera::saveToFile(std::ofstream& file)
            //open file pointer for writing:
            glout << CONTROLOFF;
            testCharacterPrinting();
            glout << CONTROLON;
            break;
        case GLUT_KEY_UP: // up arrow does windowBlind.open()
            cam.moveCamWithColl(Coord(1 * moveSpeed, 0, 0));
            break;
        case GLUT_KEY_DOWN:
            cam.moveCamWithColl(Coord(-1 * moveSpeed, 0, 0));
            break;
        case GLUT_KEY_RIGHT:
            cam.relRot(angle);
            break;
        case GLUT_KEY_LEFT:
            cam.relRot(Coord(0, -5, 0).degToRad());
            break;
        case GLUT_KEY_PAGE_UP:
            cam.relRot(Coord(5, 0, 0).degToRad());
            break;
        case GLUT_KEY_PAGE_DOWN:
            cam.relRot(Coord(-5, 0, 0).degToRad());
            break;
        case GLUT_KEY_HOME:
            fov += 5;
            glout << "FOV increased to: " << fov << '\n';
            break;
        case GLUT_KEY_END:
            fov -= 5;
            glout << "FOV decreased to: " << fov << '\n';
            break;
        case GLUT_KEY_F11:
            if (modifiers & GLUT_ACTIVE_ALT) {
                selectLockingEnabled = !selectLockingEnabled;
                glout << "Select Locking: " << (selectLockingEnabled ? "Enabled" : "Disabled") << '\n';
            } else {
                bufferPeeking = true;
            }
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
#endif

#ifndef FOLDING_REGION_ANIMATION
float cardDistFun() {
    cardDist = cam.pos.dist(Coord(18.6000004, 1.84333336, 0.25));
    return cardDist;
}

void doorAnimate() {
    if (animateDoor == DOOR_OPENING) {
        doorOpenPercent += 1;
        if (doorOpenPercent >= 100) {
            animateDoor = DOOR_OPENED_STOPPED;
            doorOpenPercent = 100;
        }
    } else if (animateDoor == DOOR_CLOSING) {
        doorOpenPercent -= 1;
        if (doorOpenPercent <= 0) {
            animateDoor = DOOR_CLOSED_STOPPED;
            doorOpenPercent = 0;
        }
    }
}

void cardAnimate() {
    if (cardRotState == CARD_ROT_NOW) {
        // cardRotState++;
        cardRotPercent++;
        if (cardRotPercent >= 100 * cardRotSpeed) {
            cardRotState = CARD_ROT_COMPLETE;
            // glout << "Card Done!" << std::endl;
        }
    } else if (cardRotState == CARD_ROT_UNDO) {
        cardRotPercent--;
        if (cardRotPercent <= 0) {
            cardRotState = CARD_ROT_NONE;
            winner = useTimeToSeedRandomToSetWinner();
            // glout << "Card Done!" << std::endl;
        }
    }
}

void animBlinds() {
    if (animatingBlinds == DOOR_OPENING) {
        windowBlinds.open(0.01);
        if (windowBlinds.closedFactor <= 0.00001) {
            animatingBlinds = DOOR_OPENED_STOPPED;
            windowBlinds.closedFactor = 0;
        }
    } else if (animatingBlinds == DOOR_CLOSING) {
        windowBlinds.close(0.01);
        if (windowBlinds.closedFactor >= 1) {
            animatingBlinds = DOOR_CLOSED_STOPPED;
            windowBlinds.closedFactor = 1;
        }
    }
}


int peek = 0;
int numPeeks = 1000;

void peekHiddenBuffer() {
    //peek the hidden buffer for numPeeks iterations of the animation function:
    if (peek < numPeeks) {
        bufferPeeking = true;
        peek++;
        debugMap[27] = "Peeking Buffer; " + std::to_string(peek) + "/" + std::to_string(numPeeks);
    } else {
        bufferPeeking = false;
        peek = 0;
        debugMap[27] = "";
    }
}

//right-click menu:
void menu(int id) {
    switch (id) {
        case 2: winner = 1;
            glout << "Winner is now " << retWinner() << '\n';
            break;
        case 3: winner = 0;
            glout << "Winner is now " << retWinner() << '\n';
            break;
        case 4: winner = useTimeToSeedRandomToSetWinner();
            glout << "Winner is now " << retWinner() << '\n';
            break;
    }
}

void quitMenu(int id) {
    switch (id) {
        case 1: exit(0);
            break;
    }
}

void top_menu(int id) {
    if (id == 1) exit(0);
}


void makeMenu(void) {
    int menui = glutCreateMenu(menu);
    glutAddMenuEntry("Set Winner to Username", 2);
    glutAddMenuEntry("Set Winner to Day of Week", 3);
    glutAddMenuEntry("Set Winner to Random", 4);


    glutCreateMenu(top_menu);
    glutAddSubMenu("Winner Menu", menui);


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void animate(int value) {
    // add value to the debugMap
    debugMap[60 - 5] = "MysteryValue: " + std::to_string(value);

    doorAnimate();
    if (cardRotState == CARD_ROT_NOW || CARD_ROT_UNDO) {
        cardAnimate();
    }

    if (animatingBlinds == DOOR_OPENING || animatingBlinds == DOOR_CLOSING) {
        animBlinds();
    }
    if (bufferPeeking) {
        peekHiddenBuffer();
    }

    glutTimerFunc(5, animate, 1);
    glutPostRedisplay();
}
#endif


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(totalWidth, height);
    glutInitWindowPosition(10, 100);
    glutCreateWindow("Scene Display Window");
    setup();
    // setupObjects();
    glutDisplayFunc(drawWindow);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(NULL);
    glutSpecialFunc(specialKeyboard);
    glutMouseFunc(mouseControl);
    makeMenu();

    glutTimerFunc(5, animate, 1);

    for (const std::string &i: instructionVec) {
        std::cout << i << '\n';
    }

    glutMainLoop();
    return 0;
}



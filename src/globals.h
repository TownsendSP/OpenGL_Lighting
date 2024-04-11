//
// Created by tgsp on 4/4/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef __APPLE__
# include <GLUT/glut.h>
#else

# include <GL/glut.h>

#endif
#include <cstdint>
#include <streambuf>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <map>
#include <vector>

#ifndef FOLDING_REGION_GLOBALDEFS


#define PI 3.14159f
#define D(radians) ((radians) * 180.0 / PI)
#define R(degrees) ((degrees) * PI / 180.0)

#define WiREFRAME 0x19
#define SOLID 0x1A

//used for determining whether to convert color values to 0-1 range
#define PLZCONVERT 0b1
#define NOCONVERT 0b0

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
#include <map>
#include <string>
#include <vector>

//defines for console:
#define CONTROLOFF 0x00
#define CONTROLON 0xFF
#define NOPREFIX 0x03
#define CLEARALL 0x04
#define PREFIX 0x05
#define LINEFEED 0x06
#define NEWLINE 0x09
#define DEL 0x0A
#define GROW 0x7f
#define SHRINK 0x80
#define MAX 0x81
#define DEFAULT 0x82

#endif

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

enum DebugLevel {
    NONE,
    WEAK,
    STRONG,
    ALL
};

// its getting annoying not being able to pass in arrays to glTranslateStuff
inline void glTranslatefv(float in[3]) {
    glTranslatef(in[0], in[1], in[2]);
};

inline void glScalefv(float in[3]) {
    glScalef(in[0], in[1], in[2]);
};

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
    const std::vector<std::string>& getConsole() const;
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
    const std::map<int, std::string>& getInfo() const;
};

extern GLInfo glStatus;
extern std::ostream glInfoOut;


#endif //GLOBALS_H

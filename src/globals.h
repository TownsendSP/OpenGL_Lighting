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

#define INSIDEOUT -1
#define OUTSIDEOUT 1


#define UNAMEWIN 1;
#define UNAMELOSE 0;

#define CARDROTCOMPLETE 2
#define CARDROTNOW 1
#define CARDROTNONE 0

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

const std::string cameraSaveFile = "cam.txt";

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

    operator float*() {
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

extern std::vector<thingHolder> staticPoints;


void addDbgPt(int idx, float xyz[3], float size, float weight);


std::vector<thingHolder> getDbgPts();

float* getDbgPts(int which);


int getNextPoint(int current);

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
extern float scrollVar;
#endif //GLOBALS_H

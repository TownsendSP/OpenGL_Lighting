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
#include <streambuf>
#include <string>


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


enum DebugLevel {
    NONE,
    WEAK,
    STRONG,
    ALL
};

std::map<int, std::string> debugMap;
std::vector<std::string> consoleMsgs;


// its getting annoying not being able to pass in arrays to glTranslateStuff
inline void glTranslatefv(float in[3]) {
    glTranslatef(in[0], in[1], in[2]);
};

inline void glScalefv(float in[3]) {
    glScalef(in[0], in[1], in[2]);
};


class GLStreamOut : public std::streambuf {
private:
    std::string buffer;
    std::vector<std::string> glConsole;

protected:
    virtual int_type overflow(int_type c);
    std::streambuf::int_type sync();

public:
    const std::vector<std::string>& getConsole() const;
};

extern GLStreamOut out;
extern std::ostream glOut;

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

extern GLInfo glInfo;
extern std::ostream glInfoOut;


#endif //GLOBALS_H

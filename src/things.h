//                  __      __  __    _                    __
//    ______________\ \    / /_/ /_  (_)___  ____ ______  / /_
//   / ___/ ___/ ___/\ \  / __/ __ \/ / __ \/ __ `/ ___/ / __ \
//  (__  ) /  / /__   \ \/ /_/ / / / / / / / /_/ (__  ) / / / /
// /____/_/   \___/    \_\__/_/ /_/_/_/ /_/\__, /____(_)_/ /_/
//                                        /____/

#ifndef TGSOUTHAHW4_THINGS_H
#define TGSOUTHAHW4_THINGS_H

#include <map>

#include "Camera.h"
#include "ColorData.h"
#include "Coord.h"
#include "globals.h"
#include "things.h"


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

//Library_Imports

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

//My Imports and Defines
#include <fstream>

#include "src/Coord.h"
#include "src/Camera.h"
#include "src/things.h"
#include "src/ColorData.h"

#include "src/lighting.h"
#include "src/testingFunctions.h"
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
bool selecting = false;
bool started = false;
int xClick;
int yClick;

float speed = 0.5f, sensitivity = 0.01f; // camera movement and mouse sensitivity
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
int frame = 0;
auto prevTime = std::chrono::high_resolution_clock::now();


#endif

#ifndef FOLDING_REGION_MATERIALS

//materials and lights
Light hallwayLight;
// Spotlight headlamp;
Light fakeSun;
ColorData globAmb;
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
    if (showInfoViewport) {
        glViewport(lVportW, 0, rVportW, height);
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, totalWidth, height);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, totalWidth / height, 0.0001, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);

    updateSpotlight();

    cam.lookAt();
    // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}

//update spotlight position and direction to match the camera
void backToBasicsCalculateTheDirVec(Coord directionVector) {
}

void drawLitShapes() {
    glEnable(GL_LIGHTING);
    sunLight.enable();


    headLamp.enable();

    glEnable(GL_LIGHT0);
    glPushMatrix();
    glTranslatef(-2, 0, -2);
    windowBlinds.draw(ALL);
    glPopMatrix();
    glDisable(GL_LIGHT0);

    drawHall();


    headLamp.enable();

    ceilingMat.apply();
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glRotatef(90, 1, 0, 0);
    glutSolidTorus(25, 50, 150, 100);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, 10, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(25, 50, 250, 100);
    glPopMatrix();
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


#endif

bool shownKeybinds = false;

// fps function
void calculateFPS() {
    frame++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - prevTime);
    if (duration.count() >= 1) {
        debugMap[60 - 3] = " FPS: " + std::to_string(frame);
        debugMap[30] = " FOV: " + std::to_string(fov);
        frame = 0;
        prevTime = currentTime;
    }
}

void showKeybindings() {
    glout << NOPREFIX;
    glout << CLEARALL;
    glout << MAX;
    for (const std::string &i: instructionVec) {
        glout << i << '\n';
    }
}

void drawWindow() {
    if (showInfoViewport) {
        setupLeft();
        calculateFPS();
        if (!shownKeybinds) {
            showKeybindings();
            shownKeybinds = true;
        }
        drawLeft();
    }
    setupRight();

    drawUnlitShapes();
    // drawMoreShapes();

    headLamp.enable();
    glEnable(GL_LIGHT4);
    glEnable(GL_LIGHT5);
    glEnable(GL_LIGHT6);

    drawLitShapes();


    glutSwapBuffers();
}

void setupObjects() {
    cam = Camera(Coord(0, 10, 0), Coord(-1, 10, 0), Coord(0, 1, 0));
    debugXes.emplace_back(Coord(0, 0, 0), 100, 2);
    windowBlinds = Blinds(1, 2, 0.1, 30);

    // hallLight = Light(

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
    float globAmb[] = {0.3, 0.3, 0.3, 1.0}; // Cool global ambient light

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
    globAmb = {0.3, 0.35, 0.3, 1.0}; // Cool global ambient light
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

//control
#ifndef FOLDING_REGION_Control

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

bool useCaps = false;

void keyboard(unsigned char key, int x, int y) {
    modifiers = glutGetModifiers();

    switch (key) {
        case 'w': //CAMERA FORWARD
            cam.relTrans(Coord(1 * speed, 0, 0));
            break;
        case 's': //CAMERA BACKWARD
            cam.relTrans(Coord(-1 * speed, 0, 0));
            break;
        case 'a': //CAMERA LEFT
            cam.relTrans(Coord(0, 0, -1 * speed));
            break;
        case 'd': //CAMERA RIGHT
            cam.relTrans(Coord(0, 0, 1 * speed));
            break;
        case 'c': //CAMERA DOWN
            cam.relTrans(Coord(0, -1 * speed, 0));
            break;
        case 'f': //CAMERA UP
            cam.relTrans(Coord(0, 1 * speed, 0));
            break;
        case 'r': //reset all but the camera
            glout << MAX;
            break;
        case 'R': //reset all
            glout << DEFAULT;
        // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);

            break;
        case ' ': //Toggle Mouse control of Camera
            toggleMouse();
            glout << "Mouse Control: " << (useMouse ? "Enabled" : "Disabled; Use ← →  and PG↑↓") << '\n';
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
            if(modifiers & GLUT_ACTIVE_ALT) {
                enabledFaces = enabledFaces | ALL_FACE;
                glout << "All Faces Enabled" << '\n';
            } else {
                dbgNormals = nextDbgState();
                glout << "NormalColorization: " << dbgNormMap[dbgNormals] << '\n';
            }
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
            glout << "Headlamp switched " << headLamp.enabled ? "On\n" : "Off\n";
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
            switch (animateDoor) {
                case DOOR_CLOSED_STOPPED:
                    glout << "Door Opening" << '\n';
                    animateDoor = DOOR_OPENING;
                    break;
                case DOOR_OPENED_STOPPED:
                    glout << "Door Closing" << '\n';
                    animateDoor = DOOR_CLOSING;
                    break;
                case DOOR_OPENING:
                    animateDoor = DOOR_OPENED_STOPPED;
                    glout << "Door Opened" << '\n';
                    break;
                case DOOR_CLOSING:
                    glout << "Door Closed" << '\n';
                    animateDoor = DOOR_CLOSED_STOPPED;
                    break;
                default:
                    break;
            }
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
            if (cardRotState == CARDROTNONE) {
                cardRotState = CARDROTNOW;
            } else {
                cardRotState = CARDROTNONE;
                cardRotPercent = 0;
                glout << "CardReset" << std::endl;
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
            windowBlinds.open(blindAnimSpeed);
            glout << "Blinds Opened" << '\n';
            break;
        case // up arrow does windowBlind.open()
        GLUT_KEY_DOWN:
            windowBlinds.close(blindAnimSpeed);
            glout << "Blinds Closed" << '\n';
            break;
        case GLUT_KEY_RIGHT:
            // Coord angle = Coord(0, 0.0349066, 0); //2 degrees
            // Coord angle = Coord(0, 0.0872665, 0); //5 degrees
            // Coord radianAngle = angle.degToRad()
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
            break;
        case GLUT_KEY_END:
            fov -= 5;
            break;

        default:
            break;
    }
    glutPostRedisplay();
}
#endif

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
    if (cardRotState == CARDROTNOW) {
        // cardRotState++;
        cardRotPercent++;
        if (cardRotPercent >= 100 * cardRotSpeed) {
            cardRotState = CARDROTCOMPLETE;
            glout << "Card Done!" << std::endl;
        }
    }
}

void animate(int value) {
    // add value to the debugMap
    debugMap[60 - 5] = "MysteryValue: " + std::to_string(value);

    doorAnimate();
    if (cardRotState == CARDROTNOW) {
        cardAnimate();
    }


    debugMap[60-4] = "ScrollVar: " + std::to_string(scrollVar);
    glutTimerFunc(5, animate, 1);
    glutPostRedisplay();
}

void mouseControl(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        xClick = x;
        yClick = y;
        selecting = true;
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        selecting = false;
    }
    glutPostRedisplay();
}

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

    glutTimerFunc(5, animate, 1);

    for (const std::string &i: instructionVec) {
        std::cout << i << '\n';
    }

    glutMainLoop();
    return 0;
}

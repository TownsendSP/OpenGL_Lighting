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

MatLib mat = MatLib();


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
//function pointer to infoVP addDebugString

std::map<int, std::string> debugMap;


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

//variables for FPS Counter:
int frame = 0;
auto prevTime = std::chrono::high_resolution_clock::now();


#endif

#ifndef FOLDING_REGION_MATERIALS



//materials and lights
Light hallwayLight;
Light sunlight;
Spotlight headlamp;
Light fakeSun;
#endif

#ifndef FOLDING_REGION_Draw

void setupRight() {
    if (showInfoViewport) {
        glViewport(lVportW, 0, rVportW, height);
    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, totalWidth, height);
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, totalWidth / height, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
    glEnable(GL_LINE_SMOOTH);
    // glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);


    cam.lookAt();

    // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
}

void drawMoreShapes() {
    // Light properties for spotlight
    GLfloat test_light_position_spotlight[] = {0.0f, 3.0f, 0.0f, 1.0f}; // Include positional component (w = 1.0)
    GLfloat test_light_diffuse_spotlight[] = {1.0f, 0.0f, 1.0f, 1.0f}; // Slightly cool color
    GLfloat test_light_specular_spotlight[] = {0.0f, 1.0f, 0.0f, 1.0f}; // Slightly cool color
    GLfloat test_light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat test_spotExponent = 1.0f;


    // Light properties for point light
    GLfloat test_light_position_pointlight[] = {-2.0f, 3.0f, -2.0f, 1.0f}; // Include positional component (w = 1.0)
    GLfloat test_light_diffuse_pointlight[] = {0.9f, 0.8f, 0.1f, 1.0f}; // Warm color

    // glDisable(GL_LIGHT0);

    // Set material properties
    // glMaterialfv(GL_FRONT, GL_AMBIENT, testmaterial_ambient);
    // glMaterialfv(GL_FRONT, GL_DIFFUSE, test_material_diffuse);
    // glMaterialfv(GL_FRONT, GL_SPECULAR, test_material_specular);
    // glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    mat.wall.apply();

    // ground plane (y = -0.5)
    glPushMatrix();

    glEnable(GL_LIGHT1);
    drawPlane(Coord(0, 0, -2), Coord(10, 0, 2), Coord(0, 1, 0), 50); //floor
    drawPlane(Coord(0, 3, -2), Coord(10, 3, 2), Coord(0, -1, 0), 50); //ceiling
    // drawPlane(Coord(0.1, 0, -2), Coord(0, 3, 2), Coord(1, 0, 0), 20); //- Back wall:
    // drawPlane(Coord(0, 0, 2), Coord(10, 3, 2.1), Coord(0, 0, -1), 50); //- Right Wall:
    // drawPlane(Coord(0, 0, -2.1), Coord(10, 3, -2), Coord(0, 0, 1), 50); //- left Wall:

    glPopMatrix();
    glDisable(GL_LIGHT1);


    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void drawLitShapes() {
    glPushMatrix();
    glTranslatef(-2, 0, -2);
    windowBlinds.draw(ALL);
    glPopMatrix();


    // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, windowBlinds.matSpecBlinds);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, windowBlinds.matShineBlinds);
    // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, windowBlinds.matAmbAndDifBlinds);
    // glPushMatrix();
    // glTranslatef(2, 0, 2);
    // glutSolidDodecahedron();
    // glPopMatrix();
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

    // testConeArot();

    // testCamBindings();

    // testDrawingCubes();
    windowTest();

    glEnable(GL_LIGHTING);
}


#endif



// fps function
void calculateFPS() {
    frame++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - prevTime);
    if (duration.count() >= 1) {
        // std::cout << "FPS: " << frame << std::endl;
        //add to debugMap
        debugMap[60 - 3] = " FPS: " + std::to_string(frame);
        debugMap[30] = " FOV: " + std::to_string(fov);
        frame = 0;
        prevTime = currentTime;
    }
}

void drawWindow() {
    if (showInfoViewport) {
        setupLeft();
        calculateFPS();
        drawLeft();
    }
    setupRight();
    drawUnlitShapes();
    // drawMoreShapes();
    // drawLitShapes();


    glutSwapBuffers();
}

void setupObjects() {
    cam = Camera(Coord(0, 7, 0), Coord(0, 7, 0), Coord(0, 1, 0));
    debugXes.emplace_back(Coord(0, 0, 0), 100, 2);
    windowBlinds = Blinds(1, 2, 0.1, 15);

    //giving them access to the debugging info map
    cam.setDebugStringAdd(&debugMap);
    windowBlinds.setDebugStringAdd(&debugMap);

    //setup lvp class:
}

void setup() {
    // Light property vectors.
    float lightAmb[] = {0.8, 0.7, 0.2, 1.0}; // Warm ambient light
    float lightDifAndSpec[] = {0.8, 0.7, 0.2, 1.0}; // Warm diffuse and specular light
    float lightPos[] = {0.0, 7.0, 0.0, 0.0}; // Position remains the same
    float globAmb[] = {0.3, 0.3, 0.3, 1.0}; // Cool global ambient light

    // Light0 properties. //light 0 is a
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Poperties of the ambient light.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.

    glEnable(GL_LIGHTING); // Enable lighting calculations.
    glEnable(GL_LIGHT0); // Enable particular light source.
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE); // Enable automatic normalization of normals.
    glShadeModel(GL_SMOOTH);

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

void showKeybindings() {
    glout << NOPREFIX;
    glout << CLEARALL;
    glout << MAX;
    for (const std::string &i: instructionVec) {
        glout << i << '\n';
    }
}

void hideKeybindings() {
    glout << CLEARALL;
    glout << DEFAULT;
    glout << PREFIX;
}

//control
#ifndef FOLDING_REGION_Control

void mouse(int x, int y) {
    // debug_strings[0] = ("Cursor: " + std::to_string(x) + ", " + std::to_string(y));

    static bool firstMouse = true;
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
    // if(1<=(int)key <=5) {
    //     cam.restoreState(((int)key)-1);
    //     glout << "Camera State " << key << " Restored" << std::endl;
    //     glout << "Cam Pos: " << cam.pos.toString() << " Cam Tgt: " << cam.tgt.toString();
    //
    // }
    if(showKeybinds) {
        if(key == '?') {
            showKeybinds = false;
            hideKeybindings();
        }
        else {
            showKeybindings();
        }
    }

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

        case '1': cam.restoreState(0);
            glout << "Camera State 1 Restored" << '\n';
            glout << "Pos: " << cam.pos.toString(0) << "Tgt: " << cam.tgt.toString(0) << '\n';
            break;
        case '2': cam.restoreState(1);
            glout << "Camera State "<<key<<" Restored" << '\n';
            glout << "Pos: " << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            break;
        case '3': cam.restoreState(2);
        glout << "Camera State "<<key<<" Restored" << '\n';
        glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            break;
        case '4': cam.restoreState(3);
        glout << "Camera State "<<key<<" Restored" << '\n';
        glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            break;
        case '5': cam.restoreState(4);
        glout << "Camera State "<<key<<" Restored" << '\n';
        glout << "Pos:" << cam.pos.toString(0) << " Cam Tgt: " << cam.tgt.toString(0) << '\n';
            break;
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
        case '?': //print keybinds:
            if(showKeybinds){hideKeybindings();showKeybinds = false;}
            else{showKeybindings();showKeybinds = true;}
            break;

        case 27: //Escape Key: Exit
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int counter = 0;

void testCharacterPrinting() {
    for (unsigned int i = counter * 5; i <= 255; i++) {
        // Print newline and increment counter every 5 characters
        // Print character hex value (formatted as 2-digit hex)
        glout << std::hex << std::setw(2) << std::setfill('0') << i << ": ";

        // Print character itself
        glout << (char)i << " ";
        if (i % 5 == 4) {
            glout << '\n';
            counter++;
            break;
        }
        if(counter >= 0xFE || i >= 0xFE)
            counter = 0;
    }
}

std::string cameraSaveFile = "cameraSave.txt";
void specialKeyboard(int key, int x, int y) {
    std::ofstream fptro(cameraSaveFile, std::ios::out);
    std::ifstream fptri(cameraSaveFile, std::ios::in);
    switch (key) {
        case GLUT_KEY_F1:
            if(!showKeybinds){
                showInfoViewport = !showInfoViewport;
                resize(totalWidth, height);
            } else {
                glout << "Cannot toggle Information Panel while Keybindings are displayed" << '\n';
                glout << "Dismiss Keybinds with ? to toggle Information Panel" << '\n';
            }

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
            glout << 0x00;
        std::cout << "CONTROLOFF 0x00"<< "   AsHex: " << xx8(CONTROLOFF) << std::endl;
        std::cout << "CONTROLON 0xFF"<< "   AsHex: " << xx8(CONTROLON) << std::endl;
        std::cout << "NOPREFIX 0x03"<< "   AsHex: " << xx8(NOPREFIX) << std::endl;
        std::cout << "PREFIX 0x05"<< "   AsHex: " << xx8(PREFIX) << std::endl;
        std::cout << "CLEARALL 0x04"<< "   AsHex: " << xx8(CLEARALL) << std::endl;
        std::cout << "LINEFEED 0x06"<< "   AsHex: " << xx8(LINEFEED) << std::endl;
        std::cout << "NEWLINE 0x09"<< "   AsHex: " << xx8(NEWLINE) << std::endl;
        std::cout << "DEL 0x08"<< "   AsHex: " << xx8(DEL) << std::endl;
        std::cout << "GROW 0x7f"<< "   AsHex: " << xx8(GROW) << std::endl;
        std::cout << "SHRINK 0x80"<< "   AsHex: " << xx8(SHRINK) << std::endl;
        std::cout << "MAX 0x81"<< "   AsHex: " << xx8(MAX) << std::endl;
        std::cout << "DEFAULT 0x82"<< "   AsHex: " << xx8(DEFAULT) << std::endl;
        break;


        case GLUT_KEY_F9: //call Camera::saveToFile(std::ofstream& file)
            //open file pointer for writing:
                cam.saveToFile(fptro);

                glout << "Camera states saved to " << cameraSaveFile << '\n';
        break;
        case GLUT_KEY_F12: //call Camera::saveToFile(std::ofstream& file)
            //open file pointer for writing:
                glout << CONTROLOFF;
                testCharacterPrinting();
                glout << CONTROLON;
        break;
        case GLUT_KEY_F5:
            cam.loadFromFile(fptri);

        glout << "Camera Profiles loaded from " << cameraSaveFile << '\n';
        glout << "Available States: " << '\n';
        for (int i = 0; i < 5; i++) {
            glout << i+1 << ": " << "Pos:" << std::get<0>(cam.storedStates[i]).toString(0) << " Tgt:" << std::get<1>(cam.storedStates[i]).toString(0) << '\n';
        }

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
            cam.relRot(Coord(0, 5, 0).degToRad());
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

void animate(int value) {
    coneRotAnim = coneRotAnim.wrap(180, -180, Coord(
                                       animInfo & 0b100 ? coneRotSpeed : 0,
                                       animInfo & 0b010 ? coneRotSpeed : 0,
                                       animInfo & 0b001 ? coneRotSpeed : 0
                                   ));
    // add value to the debugMap
    debugMap[60 - 5] = "MysteryValue: " + std::to_string(value);

    glutTimerFunc(5, animate, 1);

    glutPostRedisplay();
}

int testMain() {
    Coord a = Coord(1, 1, 1);
    Coord b = a*2;

    std::cout << (a-(b&2)).toString();
    std::cout <<a.toString();
    std::cout <<b.toString();
    exit(0);
}


int main(int argc, char **argv) {
    std::ofstream fptro(cameraSaveFile, std::ios::out);
    std::ifstream fptri(cameraSaveFile, std::ios::in);
    // testMain();
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
    // glutMouseFunc(mouseControl);

    glutTimerFunc(5, animate, 1);


    for (const std::string &i: instructionVec) {
        std::cout << i << '\n';
    }


    glutMainLoop();
    fptro.close();
fptri.close();


    return 0;
}

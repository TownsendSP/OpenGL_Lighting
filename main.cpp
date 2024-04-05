//Library_Imports

#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif
#define PI 3.14159

#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <functional>

//My Imports and Defines
#include "src/Coord.hpp"
#include "src/Camera.hpp"
#include "src/LeftVP.h"
#include "src/things.h"
#include "src/ColorData.h"
#include "src/globals.h"

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS


#ifndef FOLDING_REGION_Vport
int totalHeight = 900;
int totalWidth = 1900;
int lVportW = 300;

int lVportH = 900;
int rVportW = 1600;
int rVportH = 900;
#endif


#ifndef FOLDING_REGION_Global_Objects

Camera cam = Camera();
LeftVP infoVP = LeftVP(lVportW, lVportH, Coord(0.02745, 0.21176, 0.25882), Coord(0.51373, 0.58039, 0.58824));
std::vector<Debug3Dx> debugXes;
Blinds windowBlinds;

#endif


#ifndef FOLDING_REGION_Global_Variables
//Global Variables
bool useMouse = false;
bool selecting = false;
bool started = false;
int xClick;
int yClick;

float speed = 0.5f, sensitivity = 0.01f; // camera movement and mouse sensitivity
float blindAnimSpeed = 0.05;
bool showInfoViewport = false;
//function pointer to infoVP addDebugString


std::function<int(int, std::string)> addDebugString = std::bind(&LeftVP::addDebugString, &infoVP, std::placeholders::_1,
                                                                std::placeholders::_2);

std::vector<std::string> instructionVec = {
    "======Keybinds======",
    "Scream Internally and resist the urge to throw your computer out the window"
};

//mid-dark grey, kinda like blender's default background
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};

DebugLevel defaultDebug = WEAK;

#endif


void setupRight() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (showInfoViewport) {
        glViewport(lVportW, 0, rVportW, rVportH);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float) rVportW / (float) rVportH, 1.0, 500.0);
    } else {
        // glViewport(0, 0, totalWidth, totalHeight);
        // glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // gluPerspective(60.0, (float) totalWidth / (float) totalHeight, 1.0, 500.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam.lookAt();

    glClearColor(rVPColor[0], rVPColor[1], rVPColor[2], rVPColor[3]);
}

void drawLitShapes() {
    glPushMatrix();
    glTranslatef(-2, 0, -2);
    windowBlinds.draw(ALL);
    glPopMatrix();


    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, windowBlinds.matSpecBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, windowBlinds.matShineBlinds);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, windowBlinds.matAmbAndDifBlinds);
    glPushMatrix();
    glTranslatef(2, 0, 2);
    glutSolidDodecahedron();
    glPopMatrix();
}

void drawUnlitShapes() {
    glDisable(GL_LIGHTING);

    if (defaultDebug != NONE) {
        glPushMatrix();
        drawXZxGridlines(500);
        glPopMatrix();


        glPushMatrix();
        for (Debug3Dx debug_x: debugXes) {
            debug_x.draw();
        }
        glPopMatrix();
    }


    //unit cube
    glPushMatrix();
    glTranslatef(2, 0, 0);
    glutSolidDodecahedron();
    glPopMatrix();


    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

void drawWindow() {
    if(showInfoViewport){
        infoVP.drawViewport();
    }
    setupRight();
    drawLitShapes();
    drawUnlitShapes();
}


void setupObjects() {
    cam = Camera(Coord(10, 10, 10), Coord(0, 0, 0), Coord(0, 1, 0));
    debugXes.emplace_back(Debug3Dx(Coord(0, 0, 0), 500, 2));
    windowBlinds = Blinds(1, 2, 0.1, 15);

    cam.setDebugStringAdd(addDebugString);
}

void setup() {
    // Light property vectors.
float lightAmb[] = {0.8, 0.7, 0.2, 1.0}; // Warm ambient light
float lightDifAndSpec[] = {0.8, 0.7, 0.2, 1.0}; // Warm diffuse and specular light
float lightPos[] = {0.0, 7.0, 0.0, 0.0}; // Position remains the same
float globAmb[] = {0.2, 0.2, 0.8, 1.0}; // Cool global ambient light

    // Light0 properties. //light 0 is a
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Poperties of the ambient light.
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

    glEnable(GL_LIGHTING); // Enable lighting calculations.
    glEnable(GL_LIGHT0); // Enable particular light source.
    glEnable(GL_DEPTH_TEST); // Enable depth testing.
    glEnable(GL_NORMALIZE); // Enable automatic normalization of normals.

    setupObjects();
    glClearColor(rVPColor[0], rVPColor[1], rVPColor[2], rVPColor[3]);
}

void resize(int w, int h) {
    totalWidth = w;
    totalHeight = h;


    infoVP.lVportW = lVportW = 0.2 * totalWidth; // 20% of total width
    infoVP.lVportH = totalHeight;

    rVportW = totalWidth - lVportW;
    rVportH = totalHeight;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) w / (float) h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
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

void keyboard(unsigned char key, int x, int y) {
    Coord newFlowerCoord;
    switch (key) {
        case 'W': //CAMERA FORWARD
            cam.relTrans(Coord(1 * speed, 0, 0));
            break;
        case 'S': //CAMERA BACKWARD
            cam.relTrans(Coord(-1 * speed, 0, 0));
            break;
        case 'A': //CAMERA LEFT
            cam.relTrans(Coord(0, 0, -1 * speed));
            break;
        case 'D': //CAMERA RIGHT
            cam.relTrans(Coord(0, 0, 1 * speed));
            break;
        case 'r': //reset all but the camera
            setup();
            glClearColor(rVPColor[0], rVPColor[1], rVPColor[2], rVPColor[3]);

            break;
        case 'R': //reset all
            started = false;
            setup();
            glClearColor(rVPColor[0], rVPColor[1], rVPColor[2], rVPColor[3]);

            break;
        case 'C': //CAMERA DOWN
            cam.relTrans(Coord(0, -1 * speed, 0));
            break;
        case 'F': //CAMERA UP
            cam.relTrans(Coord(0, 1 * speed, 0));
            break;

        case ' ': //Toggle Mouse control of Camera
            toggleMouse();
            break;
        case 27: //Escape Key: Exit
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_F1:
            showInfoViewport = !showInfoViewport;
        resize(totalWidth, totalHeight);
            break;

        case GLUT_KEY_UP: // up arrow does windowBlind.open()
            windowBlinds.open(blindAnimSpeed);
            break;
        case // up arrow does windowBlind.open()
            GLUT_KEY_DOWN:
            windowBlinds.close(blindAnimSpeed);
            break;

        default:
            break;
    }
    glutPostRedisplay();
}
#endif


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(totalWidth, totalHeight);
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

    // glutTimerFunc(5, animate, 1);


    for (const std::string &i: instructionVec) {
        std::cout << i << std::endl;
    }


    glutMainLoop();


    return 0;
}

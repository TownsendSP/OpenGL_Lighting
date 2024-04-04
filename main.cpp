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

//My Imports
#include "src/Camera.hpp"
#include "src/Coord.hpp"
#include "src/LeftVP.h"
#include "src/things.h"

//ViewportDefinitions
int totalHeight = 900;
int totalWidth = 1900;
int lVportW = 300;

int lVportH = 900;
int rVportW = 1600;
int rVportH = 900;


//Global Objects
Camera cam = Camera();
LeftVP infoVP = LeftVP(lVportW, lVportH, Coord(0.02745, 0.21176, 0.25882), Coord(0.51373, 0.58039, 0.58824));
std::vector<Debug3Dx> debugXes;
Blinds windowBlinds;


//endGlobalObjs


//Global Variables
bool useMouse = true;
bool selecting = false;
bool started = false;
int xClick;
int yClick;

float speed = 0.5f, sensitivity = 0.01f; // camera movement and mouse sensitivity
bool showInfoViewport = false;
//function pointer to infoVP addDebugString
std::function<int(int, std::string)> addDebugString = std::bind(&LeftVP::addDebugString, &infoVP, std::placeholders::_1, std::placeholders::_2);

std::vector<std::string> instructionVec = {
        "======Keybinds======",
        "Scream Internally and resist the urge to throw your computer out the window"
};

//mid-dark grey
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};



void setupRight() {
    if(showInfoViewport){
        glViewport(lVportW, 0, rVportW, rVportH);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float) rVportW / (float) rVportH, 1.0, 500.0);
    } else {
        glViewport(0, 0, totalWidth, totalHeight);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, (float) totalWidth / (float) totalHeight, 1.0, 500.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam.lookAt();

    glClearColor(0.8f, 0.8f, 1.0f, 0.0f);
}

void drawLitShapes() {
    windowBlinds.draw();


}

void drawUnlitShapes() {
    glDisable(GL_LIGHTING);

    drawXZxGridlines(20);
    for (Debug3Dx debug_x: debugXes) {
        debug_x.draw();
    }
        glEnable(GL_LIGHTING);
}

void drawWindow(){
    if(showInfoViewport){
        infoVP.drawViewport();
    }
    setupRight();
}



void setupObjects() {
    cam = Camera(Coord(10, 10, 10), Coord(0, 0, 0), Coord(0, 1, 0));
    debugXes.emplace_back(Debug3Dx(50, 2, Coord(0, 0, 0)));
    windowBlinds = Blinds(1, 2, 0.1, 15, 0);


}

void setup(){


    // Light property vectors.
    float lightAmb[] = {0.0, 0.1, 1.0, 1.0};
    float lightDifAndSpec[] = {0.0, 0.1, 1.0, 1.0};
    float lightPos[] = {0.0, 7.0, 3.0, 0.0};
    float globAmb[] = {0.2, 0.2, 0.2, 1.0};

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
    glClearColor(0.3, 0.3, 0.3, 1.0);
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
        case 'W':
            cam.relTrans(Coord(1 * speed, 0, 0));
        break;
        case 'A':
            cam.relTrans(Coord(-1 * speed, 0, 0));
        break;
        case 'S':
            cam.relTrans(Coord(0, 0, -1 * speed));
        break;
        case 'D':
            cam.relTrans(Coord(0, 0, 1 * speed));
        break;
        case 'r':
            setup();
        glClearColor(0.8, 0.8, 1.0, 0.0);
        break;
        case 'R':
            started = false;
        setup();
        glClearColor(0.8, 0.8, 1.0, 0.0);
        break;
        case 'C':
            cam.relTrans(Coord(0, -1 * speed, 0));
        break;
        case 'F':
            cam.relTrans(Coord(0, 1 * speed, 0));
        break;

        case ' ': //toggleCamera
            toggleMouse();
        break;
        default:
            break;
    }
    glutPostRedisplay();
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(totalWidth, totalHeight);
    glutInitWindowPosition(10, 100);
    glutCreateWindow("Scene Display Window");
    setup();
    glutDisplayFunc(drawWindow);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glutPassiveMotionFunc(mouse);
    // glutSpecialFunc(specialKeyboard);
    // glutMouseFunc(mouseControl);

    for (const std::string &i: instructionVec) {
        std::cout << i << std::endl;
    }


    glutMainLoop();


    return 0;
}

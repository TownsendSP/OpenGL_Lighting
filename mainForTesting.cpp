//Library_Imports

#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif


#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <string>
#include <functional>
#include <map>

//My Imports and Defines
#include "src/Coord.h"
#include "src/Camera.h"
#include "src/things.h"
#include "src/ColorData.h"
#include "src/globals.h"

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

std::string* debug_strings;
std::string infoStrings[60];

std::map<int, std::string> debugMap;

std::vector<std::string> instructionVec = {
    "======Keybinds======",
    "Scream Internally and resist the urge to throw your computer out the window"
};

//mid-dark grey, kinda like blender's default background
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};
ColorData rVPColorData = ColorData(0.2, 0.2, 0.2, 1.0);
ColorData solarizedBG = ColorData(0.02745, 0.21176, 0.25882, 1.0);
ColorData solarizedText = ColorData(0.71373, 0.58039, 0.58824, 1.0);


DebugLevel defaultDebug = WEAK;

#endif

void drawLeft() {
    glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
    glDisable( GL_LIGHTING);
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


    glColor4f(solarizedText.R, solarizedText.G, solarizedText.B, solarizedText.A);
    for (const auto& pair : debugMap) {
        glRasterPos3f(3.0, (pair.first + 1) * 15, -1.0);
        std::for_each(pair.second.begin(), pair.second.end(), [](int8_t c) { glutBitmapCharacter(GLUT_BITMAP_8_BY_13, c); });
    }
    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glEnable( GL_LIGHTING);
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

void setupRight() {

    if (showInfoViewport) {
        glViewport(lVportW, 0, rVportW, height);

    } else {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, totalWidth, height);

    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float) totalWidth / (float) height, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cam.lookAt();

    // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);
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
        drawXZxGridlines(50 );
        glPopMatrix();


        glPushMatrix();

        for (Debug3Dx debug_x: debugXes) {
            debug_x.draw();
        }
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

void drawWindow() {
    if(showInfoViewport){
        setupLeft();
        drawLeft();
    }
    setupRight();
    drawLitShapes();
    drawUnlitShapes();

    glutSwapBuffers();
}

void setupObjects() {
    cam = Camera(Coord(0, 0.000000001, 0), Coord(0, 0.000000001, 0), Coord(0, 1, 0));
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
float globAmb[] = {0.2, 0.2, 0.5, 1.0}; // Cool global ambient light

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
        // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);

            break;
        case 'R': //reset all
            started = false;
            setup();
        // glClearColor(rVPColorData.R, rVPColorData.G, rVPColorData.B, rVPColorData.A);

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
            resize(totalWidth, height);
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


void loadCoord(std::string inString) {
    //x is first 20 chars, y is next 20, z is last 20
    std::stringstream Xstr (inString.substr(0, 20));
    std::stringstream Ystr (inString.substr(20, 20));
    std::stringstream Zstr (inString.substr(40, 20));
    float outX, outY, outZ;
    Xstr >> std::hexfloat >> outX;
    Ystr >> std::hexfloat >> outY;
    Zstr >> std::hexfloat >> outZ;
    Coord outCoord = Coord(outX, outY, outZ);
    std::cout << outCoord.toString(7) << std::endl;



    exit(0);


}

void testFun() {
    Coord test  = Coord(1.23, 4.56, 7.89);
    std::cout <<test.toHexString() << std::endl;
    std::string testString = test.toHexString();
    loadCoord(testString);
}


int main(int argc, char **argv) {
    testFun();
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

    // glutTimerFunc(5, animate, 1);


    for (const std::string &i: instructionVec) {
        std::cout << i << std::endl;
    }


    glutMainLoop();


    return 0;
}
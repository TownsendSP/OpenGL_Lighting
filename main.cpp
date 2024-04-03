//<editor-fold desc="Library_Imports">

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

//</editor-fold>

//My Imports
#include "src/Camera.hpp"
#include "src/Coord.hpp"
#include "src/LeftVP.h"

//<editor-fold desc="ViewportDefinitions">
int totalHeight = 900;
int totalWidth = 1900;
int lVportW = 300;
int lVportH = 900;
int rVportW = 1600;
int rVportH = 900;
//</editor-fold>

//<editor-fold desc="Global Variables">
bool useMouse = true;
bool selecting = false;
bool started = false;
int xClick;
int yClick;

float speed = 0.5f, sensitivity = 0.01f; // camera movement and mouse sensitivity
bool showInfoViewport = false;

Camera cam = Camera();


LeftVP infoVP = LeftVP(lVportW, lVportH, Coord(0.02745, 0.21176, 0.25882), Coord(0.51373, 0.58039, 0.58824));
//function pointer to infoVP addDebugString
std::function<int(int, std::string)> addDebugString = std::bind(&LeftVP::addDebugString, &infoVP, std::placeholders::_1, std::placeholders::_2);

std::vector<std::string> instructionVec = {
        "======Keybinds======",
        "Scream Internally and resist the urge to throw your computer out the window"
};

//mid-dark grey
float rVPColor[] = {0.2, 0.2, 0.2, 1.0};





void drawRight() {
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

    drawRvPort();
}

void drawWindow(){
    if(showInfoViewport){
        infoVP.drawViewport();
    }
    drawRight();



}

void setup(){
    cam = Camera(Coord(10, 10, 10), Coord(0, 0, 0), Coord(0, 1, 0));
// Material property vectors.
    float matSpec[] = {0.0, 1.0, 1.0, 1.0};
    float matShine[] = {50.0};
    float matAmbAndDif[] = {0.8, 0.8, 0.7, 1.0};

    // Light property vectors.
    float lightAmb[] = {0.0, 0.1, 1.0, 1.0};
    float lightDifAndSpec[] = {0.0, 0.1, 1.0, 1.0};
    float lightPos[] = {0.0, 7.0, 3.0, 0.0};
    float globAmb[] = {0.2, 0.2, 0.2, 1.0};

    // Material properties of the objects.
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matAmbAndDif);

    // Light0 properties.
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


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

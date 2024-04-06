//
// Created by tgsp on 4/5/2024.
//

#ifndef LIGHTS_H
#define LIGHTS_H
#include "src/ColorData.h"
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif



class Lights {
    enum whichLight {
        LIGHT0 = GL_LIGHT0,
        LIGHT1 = GL_LIGHT1,
        LIGHT2 = GL_LIGHT2,
        LIGHT3 = GL_LIGHT3,
        LIGHT4 = GL_LIGHT4,
        LIGHT5 = GL_LIGHT5,
        LIGHT6 = GL_LIGHT6,
        LIGHT7 = GL_LIGHT7
    };
    whichLight light;
    QuadPos pos;
    ColorData ambient;
    ColorData diffuse;
    ColorData specular;
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
};

class PointLight : public Lights {


};

class DirectionalLight : public Lights {
    Coord direction;
    float spotCutoff;
    float spotExponent;

};






#endif //LIGHTS_H

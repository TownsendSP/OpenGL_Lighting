//
// Created by tgsp on 4/4/2024.
//

#ifndef LIGHTING_H
#define LIGHTING_H
#include "ColorData.h"


class Light {
public:
    enum lightNum {
        LIGHT0 = GL_LIGHT0,
        LIGHT1 = GL_LIGHT1,
        LIGHT2 = GL_LIGHT2,
        LIGHT3 = GL_LIGHT3,
        LIGHT4 = GL_LIGHT4,
        LIGHT5 = GL_LIGHT5,
        LIGHT6 = GL_LIGHT6,
        LIGHT7 = GL_LIGHT7
    };
    ColorData lightAmb;
    ColorData lightDiff;
    ColorData lightSpec;
    ColorData lightPos;
    lightNum whichLight;



    Light(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec) {
        lightAmb = amb;
        lightDiff = diff;
        lightSpec = spec;
        lightPos = pos;
    };

    Light(lightNum whichLight, ColorData pos, ColorData amb, ColorData diffspec) {
        lightAmb = amb;
        lightDiff = diffspec;
        lightSpec = diffspec;
        lightPos = pos;
    };

    void setup();

    void enable() {
        glEnable(whichLight);
    };

    void disable() {
        glDisable(whichLight);
    };

    // define a cast so I can use the light in glEnable
    operator lightNum() const {
        return whichLight; //returns, for example, GL_LIGHT0
    };

    //default constructor:
    Light() = default;
};

class Spotlight: public Light {
public:
    Coord spotDir; //angle: X rot, Y rot, Z rot, 1.0
    float spotAttenuation;
    float spotAngle;

    Spotlight(lightNum whichLight, ColorData pos, ColorData amb, ColorData diff, ColorData spec, ColorData dir,
        float cutoff, float exponent): Light(whichLight, pos, amb, diff, spec) {
        spotDir = dir;
        spotAttenuation = exponent;
        spotAngle = cutoff;
    };


    Spotlight() = default;


    Spotlight(Light i, Coord dir, float cutoff, float exponent) : Light(i.whichLight, i.lightPos, i.lightAmb, i.lightDiff, i.lightSpec) {
        spotDir = dir;
        spotAttenuation = exponent;
        spotAngle = cutoff;
    };

void setup();

    void drawVis();
};


class Material {
public:
    Material() = default;

private:
    ColorData matSpec;
    ColorData matAmb;
    ColorData matEmission;
    ColorData matDiff;
    float matShine;

public:
    //constructor:
    Material(ColorData spec, ColorData amb, ColorData diff, ColorData emission, float shine) {
        matSpec = spec;
        matAmb = amb;
        matEmission = emission;
        matDiff = diff;
        matShine = shine;
    };

    Material(ColorData spec, ColorData amb, ColorData diff, float shine) {
        matSpec = spec;
        matAmb = amb;
        matEmission = ColorData(0, 0, 0, 0);
        matDiff = diff;
        matShine = shine;
    };

    //applyMaterial:
    void apply() {
        glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
        glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
        // if (matEmission != NULL)
        glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
        glMaterialf(GL_FRONT, GL_SHININESS, matShine);
    };

    //default constructor:

};





#endif //LIGHTING_H

//
// Created by tgsp on 4/3/2024.
//

#ifndef TGSOUTHAHW4_THINGS_H
#define TGSOUTHAHW4_THINGS_H
# include "Coord.hpp"
# include "ColorData.h"



//big comment
//   _  __       ____                __   __                __  __      __  __       _         __     __              _____         _       __ _      __ __   __
//  | |/ / _    / __ \ ___   ____   / /_ / /_               \ \/ /_    / / / /___   (_)____ _ / /_   / /_            /__  /   _    | |     / /(_)____/ // /_ / /_
//  |   / (_)  / / / // _ \ / __ \ / __// __ \   ______      \  /(_)  / /_/ // _ \ / // __ `// __ \ / __/  ______      / /   (_)   | | /| / // // __  // __// __ \
// /   | _    / /_/ //  __// /_/ // /_ / / / /  /_____/      / /_    / __  //  __// // /_/ // / / // /_   /_____/     / /__ _      | |/ |/ // // /_/ // /_ / / / /
///_/|_|(_)  /_____/ \___// .___/ \__//_/ /_/               /_/(_)  /_/ /_/ \___//_/ \__, //_/ /_/ \__/              /____/(_)     |__/|__//_/ \__,_/ \__//_/ /_/
//                       /_/                                                        /____/


// class things {
//
// };

class Blinds{
    float width;
    float height;
    float depth;
    float pitchAngle;

    float* matSpecBlinds;
    float matShineBlinds[1]{};
    float* matAmbAndDifBlinds;

    int numBlades;
    float bladeHeight = 0.01;

    float closedFactor;
    void drawBlade() const;
    bool opening = false;

public:
    Blinds(float width, float height, float depth, float pitchAngle, float closedFactor = 1.0);
    void draw() const;
    void open(float amt);
    void close(float amt);
    void animate(float amt);

    Blinds() = default;
};

class Debug3Dx {
public:
    float size;
    float weight;
    Coord* position;

    Debug3Dx(Coord* position, float size = 1.0, float weight = 1.0);
    Debug3Dx(Coord position, float size = 1.0, float weight = 1.0);

    // Debug3Dx(float size = 1.0, float weight = 1.0, Coord position);

    void draw() const;
};

void drawXZxGridlines(float range);




#endif //TGSOUTHAHW4_THINGS_H

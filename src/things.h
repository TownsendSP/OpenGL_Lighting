//
// Created by tgsp on 4/3/2024.
//

#ifndef TGSOUTHAHW4_THINGS_H
#define TGSOUTHAHW4_THINGS_H
# include "Coord.hpp"




//__  __    ____             _   _        __   __   _   _      _       _     _       _____   __        ___     _ _   _
//\ \/ /_  |  _ \  ___ _ __ | |_| |__     \ \ / /  | | | | ___(_) __ _| |__ | |_    |__  /_  \ \      / (_) __| | |_| |__
// \  /(_) | | | |/ _ \ '_ \| __| '_ \     \ V (_) | |_| |/ _ \ |/ _` | '_ \| __|     / /(_)  \ \ /\ / /| |/ _` | __| '_ \
// /  \ _  | |_| |  __/ |_) | |_| | | |_    | | _  |  _  |  __/ | (_| | | | | |_ _   / /_ _    \ V  V / | | (_| | |_| | | |
///_/\_(_) |____/ \___| .__/ \__|_| |_( )   |_|(_) |_| |_|\___|_|\__, |_| |_|\__( ) /____(_)    \_/\_/  |_|\__,_|\__|_| |_|
//                      |_|             |/                         |___/          |/




class things {

};

class Blinds{
    float width;
    float height;
    float depth;
    float pitchAngle;

    float* matSpecBlinds;
    float matShineBlinds[1];
    float* matAmbAndDifBlinds;

    float numBlades;
    float bladeWidth;
    float bladeHeight = 0.01;



public:
    Blinds(float width, float height, float depth, float pitchAngle);

    void draw();

};


#endif //TGSOUTHAHW4_THINGS_H

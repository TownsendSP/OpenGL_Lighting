//
// Created by tgsp on 4/9/2024.
//

#include "Scenedraw.h"
#include "Coord.h"
#include "ColorData.h"
#include "globals.h"
#include "things.h"
#include "lighting.h"

Coord hallBnl = Coord(0, 0, -2);
Coord halltfr = Coord(10, 3, 2);


void drawHall() {
    // MatLib.wall.apply();
    cubeOfPlanes(hallBnl, halltfr, 40, -1, BACK_FACE|LEFT_FACE|RIGHT_FACE);

}



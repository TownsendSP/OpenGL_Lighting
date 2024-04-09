//
// Created by tgsp on 4/4/2024.
//

#ifndef GLOBALS_H
#define GLOBALS_H
#include "lighting.h"

enum DebugLevel {
    NONE,
    WEAK,
    STRONG,
    ALL
};


#ifndef FOLDING_REGION_MATERIALS

inline Material wallMaterial = Material(
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.8f, 0.8f, 1.0f),
    10.0f);

inline Material floorMaterial = Material( //pale brown color
    ColorData(0.8f, 0.6f, 0.4f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.8f, 0.6f, 0.4f, 1.0f),
    33.0f);


inline Material ceilingMaterial = Material( //matte cream color
    ColorData(0.9f, 0.9f, 0.8f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(0.9f, 0.9f, 0.8f, 1.0f),
    5.0f);

inline Material lampMaterial = Material( //emissive golden color.
    ColorData(1.0f, 0.9f, 0.7f, 1.0f),
    ColorData(0.2f, 0.2f, 0.2f, 1.0f),
    ColorData(1.0f, 0.9f, 0.7f, 1.0f),
    ColorData(1.0f, 0.9f, 0.7f, 1.0f),
    5.0f);

inline Material cardMaterial = Material( // metallic blue color
    ColorData(0.3f, 0.3f, 0.7f, 1.0f),
    ColorData(0.2f, 0.2f, 0.4f, 1.0f),
    ColorData(0.1f, 0.2f, 0.7f, 1.0f),
    5.0f);
#endif



#endif //GLOBALS_H

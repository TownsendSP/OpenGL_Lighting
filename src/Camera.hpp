//

//

#ifndef CAMERA_H
#define CAMERA_H

#include <complex.h>
#include <vector>

#include "Coord.hpp"
#include "globals.h"

class Camera {
    private:
    std::function<int(int, std::string)> debug_string_add_;

public:
    Coord pos;
    Coord tgt;
    Coord up;
    Coord ang;
    float transSpeed;
    float rotSens;

    // constructor:

    Camera(float position[3], float target[3], float orientation[3], float trans_speed, float rot_sens);
    Camera(Coord pos, Coord target, Coord orientation, float trans_speed, float rot_sens);

    Camera(Coord Pos, Coord Target, Coord orientation);

    void setPitchYaw();


    Camera() = default;

    // calculating movement
    void relTrans(Coord deltaTranslation);

    void relRot(Coord deltaAngle);

    void lookAt(DebugLevel dbg = NONE);

    std::vector<std::string> toString() const;

    // std::vector<std::string> toString() const;

    void relRot(float pitch, float yaw);


    // debugString functionpointer
    std::function<int(int, std::string)> getDebugStringAdd() const {
        return debug_string_add_;
    }

    void setDebugStringAdd(const std::function<int(int, std::string)>& debug_string_add) {
        debug_string_add_ = debug_string_add;
    }

    int useDebugStringAdd(int line, std::string message);
};

Coord calcPitchYaw(Coord position, Coord target);
Coord calcTarget(Coord position, Coord pitchYaw);


#endif //CAMERA_H

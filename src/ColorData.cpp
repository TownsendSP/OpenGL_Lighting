//                  __      ______      __           ____        __
//    ______________\ \    / ____/___  / /___  _____/ __ \____ _/ /_____ _ _________  ____
//   / ___/ ___/ ___/\ \  / /   / __ \/ / __ \/ ___/ / / / __ `/ __/ __ `// ___/ __ \/ __ \
//  (__  ) /  / /__   \ \/ /___/ /_/ / / /_/ / /  / /_/ / /_/ / /_/ /_/ // /__/ /_/ / /_/ /
// /____/_/   \___/    \_\____/\____/_/\____/_/  /_____/\__,_/\__/\__,_(_)___/ .___/ .___/
//                                                                          /_/   /_/

#include "ColorData.h"




bool isColorCloseEnough(ColorData color1, ColorData color2, float closeness) {
    if (abs(color1.R - color2.R) < closeness &&
        abs(color1.G - color2.G) < closeness &&
        abs(color1.B - color2.B) < closeness) {
        return true;
        }
    return false;
}



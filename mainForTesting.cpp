
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <iostream>
#include "src/globals.h"
#include "src/Coord.h"

int main() {
    Coord roomtfr = Coord(20.0f, 5.0f, 6.0f);
    Coord br3 = Coord(1.0f, 2.0f, 3.0f);

    Coord abc = (roomtfr&101)|br3;
    std::cout << abc << std::endl;
    return 0;
}

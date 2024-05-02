cmake_minimum_required(VERSION 3.0)
project(tgsouthaHW4)

set(CMAKE_CXX_STANDARD 17)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_PREFIX_PATH "C:/Program/NVIDIA Corporation/Cg")
endif()

set(MAIN_BINARY_NAME "OpenGLWindow")
set(TESTING_BINARY_NAME "testing")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR}   ${GLUT_INCLUDE_DIRS})

add_executable(${MAIN_BINARY_NAME} main.cpp
        src/Camera.cpp
        src/Camera.h
        src/Coord.cpp
        src/Coord.h
        src/LeftVP.cpp
        src/LeftVP.h
        src/things.cpp
        src/things.h
        src/ColorData.cpp
        src/ColorData.h
        src/lighting.cpp
        src/lighting.h
        src/globals.h
        src/globals.cpp
        src/Scenedraw.cpp


)
target_link_libraries (${MAIN_BINARY_NAME}  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})


add_executable(new_target tgsouthaHW4.cpp tgsouthaHW4.cpp)

target_link_libraries (new_target  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
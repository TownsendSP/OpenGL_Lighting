cmake_minimum_required(VERSION 3.0)
project(tgsouthaHW4)

set(CMAKE_CXX_STANDARD 17)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_PREFIX_PATH "C:/Program/NVIDIA Corporation/Cg")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR}   ${GLUT_INCLUDE_DIRS})

add_executable(tgsouthaHW4 main.cpp
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
        src/testingFunctions.cpp
        src/testingFunctions.h
)
target_link_libraries (tgsouthaHW4 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})


add_executable(testing mainForTesting.cpp
        src/Coord.cpp
        src/Coord.h
#
#        src/Camera.cpp
#        src/Camera.h
#
#        src/LeftVP.cpp
#        src/LeftVP.h
#        src/things.cpp
#        src/things.h
#        src/ColorData.cpp
#        src/ColorData.h
#        src/lighting.cpp
#        src/lighting.h
#        src/globals.h
#        src/globals.cpp
#        src/Scenedraw.cpp
#        src/testingFunctions.cpp
#        src/testingFunctions.h

)

target_link_libraries (testing ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})


set(CMAKE_CURRENT_SOURCE_DIR "C:/Users/tgsp/AppData/Roaming/JetBrains/CLion2024.1/scratches")
file(GLOB SCRATCH_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(SCRATCH_SOURCE ${SCRATCH_SOURCES})
    get_filename_component(SCRATCH_NAME ${SCRATCH_SOURCE} NAME_WE)
    add_executable(${SCRATCH_NAME} ${SCRATCH_SOURCE})
    target_link_libraries(${SCRATCH_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
endforeach()
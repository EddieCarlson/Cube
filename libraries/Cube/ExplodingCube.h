#ifndef explode_cube_h
#define explode_cube_h

#include <Point.h>
#include <Cube.h>

bool isOnCube(Point center, int cubeRad, Point p);

void explodeCube(Cube* cube);

#endif

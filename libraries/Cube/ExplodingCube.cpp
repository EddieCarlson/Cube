#include <ExplodingCube.h>

bool isOnCube(Point center, int cubeRad, Point p) {
  return (abs(p.x - center.x) == cubeRad && abs(p.y - center.y) <= cubeRad && abs(p.z - center.z) <= cubeRad) ||
    (abs(p.y - center.y) == cubeRad && abs(p.x - center.x) <= cubeRad && abs(p.z - center.z) <= cubeRad) ||
    (abs(p.z - center.z) == cubeRad && abs(p.x - center.x) <= cubeRad && abs(p.y - center.y) <= cubeRad);
}

void explodeCube(Cube* cube) {
  int xSize = cube->xSize;
  int ySize = cube->ySize;
  int zSize = cube->zSize;
  Point start = Point (rand() % (xSize - 2) + 1, ySize / 2, rand() % (zSize - 2) + 1);
  Point color = Point(190, 70, 40);
  for(int t = 0; t < 10; t++) {
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        for(int z = 0; z < zSize; z++) {
          Point p = Point(x, y, z);
          if (isOnCube(start, t, p)) {
            cube->setPixel(&p, &color);
          }
        }
      }
    }
    cube->show();
    delay(100);
    cube->resetPixels();
  }
}

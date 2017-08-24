#ifndef cube_slide_h
#define cube_slide_h

#include <Point.h>

class CubeDef {
  public:
  int color;
  Point* point;

  CubeDef(Point* p, int c) {
    point = p;
    color = c;
  }

};

/* 0 - matrix green
 * 5 - less harsh green
 * 10 - slightly yelower/more normal green
 * 15 - pea green
 * 20 - chartruce
 * 25 - yellow
 * 30 - incandescent yellow
 * 35 - slightly orangier
 * 40 - yellow-oarnge
 * 45 - orange
 * 50 - red-orange
 * 55 - red-red-orange
 * 60 - red
 * 65 - neon pink
 * 70 - pink
 * 75 - purply pink
 * 80 - even purplier pink
 * 85 - purple
 * 90 - lavender
 * 95 - blue-purple
 * 100 - darker blue-purple
 * 105 - violet
 * 110 - indigo
 * 115 - blue
 * 120 - blue
 * 125 - tarp blue
 * 130 - ligher blue
 * 135 - lighter blue
 * 140 - light blue
 * 145 - robins egg
 * 150 - light aqua
 * 155 - ?
 * 160 - seafoam
 * 165 - mint
 * 170 - ?
 * 165 - ?
 * 170 - dill pickle
 * 175 - ?
 */

class CubeSlide {
  public:
  Cube* cube;
  int colorI = 0;
  int* rainbow;
  int xSize;
  int ySize;
  int zSize;
  float brightness;
  CubeDef* cubeBases[8];
  int lastMovedIndex;
  int maxDelay = 45;
  int minDelay = 18;
  int delayTime = 30;
  bool delayUp = true;
  int blockSize = 6;
  int slideSize = 6;

  CubeSlide(Cube* _cube, int* _rainbow, float _brightness, int _blockSize) {
    cube = _cube;
    rainbow = _rainbow;
    brightness = _brightness;
    xSize = cube->xSize;
    ySize = cube->ySize;
    zSize = cube->zSize;
    int lastMovedIndex = 0;

    // sliderSize = 5
    cubeBases[0] = new CubeDef(new Point(0, 0, 0), dimInt(rainbow[7], 0.2));
    cubeBases[1] = new CubeDef(new Point(0, 0, slideSize), dimInt(rainbow[28], 0.2));
    cubeBases[2] = new CubeDef(new Point(0, slideSize, 0), dimInt(rainbow[50], 0.2));
    cubeBases[3] = new CubeDef(new Point(0, slideSize, slideSize), dimInt(rainbow[67], 0.2));
    cubeBases[4] = new CubeDef(new Point(slideSize, 0, 0), dimInt(rainbow[85], 0.2));
    cubeBases[5] = new CubeDef(new Point(slideSize, 0, slideSize), dimInt(rainbow[100], 0.2));
    cubeBases[6] = new CubeDef(new Point(slideSize, slideSize, 0), dimInt(rainbow[125], 0.2));
    cubeBases[7] = new CubeDef(new Point(slideSize, slideSize, slideSize), dimInt(rainbow[160], 0.2));
  }

  bool moveIfUnblocked(Point* p, bool requireInside) {
    dirs_t dirs_ts[6] = {PX, NX, PY, NY, PZ, NZ};
    int dIndex = rand() % 6;
    int dStart = dIndex;
    dirs_t dir;
    bool blocked;
    do {
      blocked = false;
      dIndex = (dIndex + 1) % 6;
      dir = dirs_ts[dIndex];
      Point* moved = p->move(dir, slideSize);
      bool isInsideCube = cube->inCube(moved);
      if (!isInsideCube && requireInside) {
        blocked = true;
      } else {
        for (int i = 0; i < 8; i++) {
          if (cubeBases[i]) {
            bool isBlockedByCube = (cubeBases[i]->point)->equals(moved);
            if (isBlockedByCube) {
              blocked = true;
            }
          }
        }
      }
      delete moved;
    } while (blocked && dIndex != dStart);

    if (!blocked) {
      for (int j = 0; j < slideSize; j++) {
        p->move_in_place(dir);
        displayCubes();
        delay(delayTime);
      }
    }

    return blocked;
  }

  void move() {
    int cubeIndex = rand() % 8;
    bool allowDoubleMove = (rand() % 10) > 7; // 20%
    while (cubeBases[cubeIndex] == NULL || (cubeIndex == lastMovedIndex && !allowDoubleMove)) {
      cubeIndex = (cubeIndex + 1) % 8;
    }
    lastMovedIndex = cubeIndex;
    CubeDef* cd = cubeBases[cubeIndex];
    Point* p = cd->point;
    bool blocked = moveIfUnblocked(p, true);
    if (blocked && cube->inCube(p)) {
      blocked = moveIfUnblocked(p, false);
    }
    /*
    bool blocked = true;
    displayCubes();
    delay(delayTime);
    */
    if (!blocked) {
      delay(delayTime);
      if (rand() % 3 == 0) {
        if (delayTime == maxDelay) {
          delayUp = false;
        } else if (delayTime == minDelay) {
          delayUp = true;
        }
        if (delayUp) {
          delayTime += 1;
        } else {
          delayTime -= 1;
        }
      }
    }
  }

  int toUnsigned(char base) {
    if (base > 128) {
      return base - 256;
    } else {
      return base;
    }
  }

  void displayCubes() {
    for(int i = 0; i < 8; i++) {
      CubeDef* cd = cubeBases[i];
      if (cd == NULL || cd->point == NULL) {
        continue;
      }
      Point* base = cd->point;
      int xBase = toUnsigned(base->x);
      int yBase = toUnsigned(base->y);
      int zBase = toUnsigned(base->z);
      Serial.println(String(colorI));
      for(int x = xBase; x < xBase + blockSize; x++) {
        for(int y = yBase; y < yBase + blockSize; y++) {
          for(int z = zBase; z < zBase + blockSize; z++) {
            cube->setPixel(x, y, z, cd->color);
          }
        }
      }
    }
    colorI = (colorI + 5) % 180;
    cube->show();
    cube->resetPixels();
  }

};

#endif

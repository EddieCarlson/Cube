#ifndef cube_slide_h
#define cube_slide_h

#include <Point.h>

class CubeDef {
  public:
  Point* point;
  int color;

  CubeDef(Point* p, int c) {
    point = p;
    color = c;
  }

};

class CubeSlide {
  public:
  Cube* cube;
  int colorI;
  int* rainbow;
  int xSize;
  int ySize;
  int zSize;
  float brightness;
  int sliderSize;
  CubeDef* cubeBases[8];

  CubeSlide(Cube* _cube, int* _rainbow, float _brightness, int _sliderSize) {
    Serial.println("xxxxxx ");
    cube = _cube;
    colorI = 0;
    rainbow = _rainbow;
    brightness = _brightness;
    sliderSize = _sliderSize;
    delay(10);

    xSize = cube->xSize;
    ySize = cube->ySize;
    zSize = cube->zSize;

    // sliderSize = 5
    cubeBases[0] = new CubeDef(new Point(0, 0, 0), dimInt(rainbow[0], 0.2));
    Serial.println("yyyyyy ");
    delay(20);
    Serial.println("szzzzz " + String(cubeBases[0]->color));
    cubeBases[1] = new CubeDef(new Point(0, 0, 7), dimInt(rainbow[5], 0.2));
    cubeBases[2] = new CubeDef(new Point(0, 7, 0), dimInt(rainbow[5], 0.2));
    cubeBases[3] = new CubeDef(new Point(0, 7, 7), dimInt(rainbow[20], 0.2));
    cubeBases[4] = new CubeDef(new Point(7, 0, 0), dimInt(rainbow[70], 0.2));
    cubeBases[5] = new CubeDef(new Point(7, 0, 7), dimInt(rainbow[5], 0.2));
    cubeBases[6] = new CubeDef(new Point(7, 7, 0), dimInt(rainbow[120], 0.2));
    cubeBases[7] = new CubeDef(new Point(7, 7, 7), dimInt(rainbow[160], 0.2));
  }

  /*
  bool inLilCube(Point* lilCube, int x, int y, int z) {
    int lx = lilCube->x;
    int ly = lilCube->y;
    int lz = lilCube->z;
    return x > lx && y > ly && z > lz && x < lx + sliderSize && y < ly + sliderSize && z < lz + sliderSize;
  }
  */

  CubeDef* selectMover() {
    int i = rand() % 8;
    while(cubeBases[i] == NULL) i = (i + 1) % 8; // TOOD: what if they're all null?
    return cubeBases[i];
  }

  dirs_t availableDir(Point* p) {
    dirs_t dirs_ts[6] = {PX, NX, PY, NY, PZ, NZ};
    int i = rand() % 6;
    dirs_t dir = dirs_ts[i];
    bool blocked;
    do {
      blocked = false;
      i = (i + 1) % 6;
      dir = dirs_ts[i];
      Point* moved = p->move(dir, 5);
      for (int i = 0; i < 8; i++) {
        if (cubeBases[i] && (cubeBases[i]->point)->equals(moved)) {
          blocked = true;
          break;
        }
      }
      delete moved;
    } while (blocked);
    return dir;
  }

  void cubeSlide() {
    CubeDef* cubeToMove = selectMover();
    Point* pointToMove = cubeToMove->point;
    dirs_t dir = availableDir(pointToMove);
    for (int i = 0; i < sliderSize; i++) {
      pointToMove->move_in_place(dir);
      displayCubes();
      delay(1000);
    }
    delay(1000);
  }

  void displayCubes() {
    Serial.println("hix");
    for(int i = 0; i < 8; i++) {
      Serial.println(String(i));
      CubeDef* cd = cubeBases[i];
      Point* lc = cd->point;
      if (lc == NULL) {
        Serial.println("nullllll");
        continue;
      }
      for(int x = lc->x; x < lc->x + 5; x++) {
        for(int y = lc->y; y < lc->y + 5; y++) {
          for(int z = lc->z; z < lc->z + 5; z++) {
            Serial.println("c " + String(cd->color));
            cube->setPixel(x, y, z, cd->color);
          }
        }
      }
    }
    cube->show();
    cube->resetPixels();
    Serial.println("byex");
  }

};

#endif

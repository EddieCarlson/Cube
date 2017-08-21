#ifndef rain_h
#define rain_h
#include <deque>

class Rain {
  public:
  Cube* cube;
  int colorI;
  int* rainbow;
  int xSize;
  int ySize;
  int zSize;

  Rain(Cube* _cube, int* _rainbow) {
    cube = _cube;
    colorI = 0;
    rainbow = _rainbow;

    xSize = cube->xSize;
    ySize = cube->ySize;
    zSize = cube->zSize;
  }

  RainDrop* randomTopDrop() {
    Point *p = randomTopPixel();
    Point *c = new Point(220 + (rand() % 25), rand() % 20, rand() % 20);
    return new RainDrop(p, c, (rand() % 4) + 2);
  }

  RainDrop* randomTopDrop(Point* color) {
    Point *p = randomTopPixel();
    return new RainDrop(p, color, (rand() % 4) + 2);
  }

  Point *randomTopPixel() {
    return new Point(rand() % xSize, rand() % ySize, zSize - 1);
  }

  void rain(long maxTime) {
    Point color = Point(250, 50, 0);
    Point fadedColor = Point(color.x * 0.1, color.y * 0.1, color.z * 0.1);

    std::deque<RainDrop *> drops;
    RainDrop* d = randomTopDrop();
    drops.push_back(d);
    int colorI = 0;
    while(millis() < maxTime) {
      int size = drops.size();
      for(int i = 0; i < size; i++) {
        RainDrop* cur = drops.front();
        drops.pop_front();
        Point* curPix = cur->point;
        cube->setPixel(curPix, cur->color);
        Point* a = curPix->move(PZ);
        cube->setPixel(a, cur->fadedColor);
        a->move_in_place(PZ);
        cube->setPixel(a, cur->moreFadedColor);
        a->move_in_place(PZ);
        cube->setPixel(a, cur->mostFadedColor);
        cur->move();
        if (cube->inCube(cur->point) || cube->inCube(a)) {
          drops.push_back(cur);
        } else {
          delete cur;
        }
        delete a;
      }
      cube->show();
      if (rand() % 4 > 1) {
        int colorIrand = (colorI + rand() % 14) % 180;
        drops.push_back(randomTopDrop(new Point(rainbow[colorIrand])));
        if (rand() % 4 == 0) {
          colorI = (colorI + 1) % 180;
        }
      }
      delay(32);
      cube->resetPixels();
    }


    while (drops.size() > 0) {
      RainDrop* cur = drops.front();
      drops.pop_front();
      delete cur;
    }
  }

  /*
  void rain() {
    Point color = Point(250, 50, 0);
    Point fadedColor = Point(color.x * 0.1, color.y * 0.1, color.z * 0.1);

    std::deque<Point *> points;
    Point *p = randomTopPixel();
    points.push_back(p);
    while(true) {
      int size = points.size();
      Serial.println(size);
      for(int i = 0; i < size; i++) {
        Point* cur = points.front();
        points.pop_front();
        cube.setPixel(cur, &color);
        Point* a = cur->move(PZ);
        cube.setPixel(a, &fadedColor);
        delete a;
        cur->move_in_place(NZ);
        if (cube.inCube(cur)) {
          points.push_back(cur);
        } else {
          delete cur;
        }
      }
      cube.show();
      points.push_back(randomTopPixel());
      delay(100);
      cube.resetPixels();
    }
  }
  */

};

#endif

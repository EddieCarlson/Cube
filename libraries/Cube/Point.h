#ifndef point_h
#define point_h

#include <OctoWS2811.h>

enum dirs_t {PX, NX, PY, NY, PZ, NZ};

class Point {
  public:
  char x;
  char y;
  char z;

  Point(char _x, char _y, char _z) {
    x = _x;
    y = _y;
    z = _z;
  }

  Point(int c) {
    x = (c >> 16) & 0xFF;
    y = (c >> 8) & 0xFF;
    z = c & 0xFF;
  }

  // Passing by value would copy? doesn't really matter here...what's the accepted convention?
  Point average(Point *other) {
    return Point((x + other->x) / 2, (y + other->y) / 2, (z + other->z) / 2);
  }

  // TODO: dir should be enum
  // TODO: should new the returned points. (ugh but someone has to free them later. so lame. any way around that?)
  //     could return int and bitshift, but that also sucks
  Point* move(dirs_t dir) {
    return move(dir, 1);
  }

  Point* move(dirs_t dir, int amt) {
    switch(dir) {
      case PX: return new Point(x + amt, y, z); break;
      case NX: return new Point(x - amt, y, z); break;
      case PY: return new Point(x, y + amt, z); break;
      case NY: return new Point(x, y - amt, z); break;
      case PZ: return new Point(x, y, z + amt); break;
      case NZ: return new Point(x, y, z - amt); break;
      default: return new Point(-amt, -amt, -amt);
    }
  }

  bool equals(Point* p) {
    return p->x == x && p->y == y && p->z == z;
  }

  void move_in_place(dirs_t dir) {
    move_in_place(dir, 1);
  }

  void move_in_place(dirs_t dir, int amt) {
    switch(dir) {
      case PX: x += amt; break;
      case NX: x -= amt; break;
      case PY: y += amt; break;
      case NY: y -= amt; break;
      case PZ: z += amt; break;
      case NZ: z -= amt; break;
      default: return;
    }
  }

  bool equals(Point o) {
    return x == o.x && y == o.y && z == o.z;
  }

  Point intToColor(int c) {
    short red = (c << 16) & 0xFF;
    short green = (c << 8) & 0xFF;
    short blue = c & 0xFF;
    return Point(red, green, blue);
  }
};

class RainDrop {
public:
  Point* point;
  Point* color;
  Point* fadedColor;
  Point* moreFadedColor;
  Point* mostFadedColor;
  int speed;
  int speedLeft;
  RainDrop(Point* p, Point* c, int s) {
    point = p;
    color = c;
    speed = s;
    speedLeft = speed;
    fadedColor = new Point(c->x * 0.1, c->y * 0.1, c->z * 0.1);
    moreFadedColor = new Point(c->x * 0.07, c->y * 0.07, c->z * 0.07);
    mostFadedColor = new Point(c->x * 0.04, c->y * 0.04, c->z * 0.04);
  }
  ~RainDrop() {
    delete point;
    delete color;
    delete fadedColor;
    delete moreFadedColor;
    delete mostFadedColor;
  }
  void move() {
    speedLeft -= 1;
    if (speedLeft == 0) {
      speedLeft = speed;
      point->move_in_place(NZ);
    }
  }
};

#endif

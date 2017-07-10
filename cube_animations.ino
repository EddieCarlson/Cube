#include <OctoWS2811.h>

#include <Cube.h>
#include <Dumb.h>
#include <Point.h>
#include <rainbow.h>
#include <ExplodingCube.h>
#include <Sphere.h>

#include <array>
#include <math.h>
#include <iterator>
#include <list>
#include <vector>
#include <deque>

int ledPin = 13;

const int ledsPerStrip = 600;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int bottomBurn = 1;
int startBurn = 0;
int endBurn = 1;
int strandsPerPanel = 3;

const int xSize = 12;
const int ySize = 12;
const int zSize = 12;

Cube cube(leds, xSize, ySize, zSize);

int rainbow[180];

Sphere sphere(&cube, rainbow);

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  delay(1000);
  setupRainbow(rainbow);
  cube.setUp(strandsPerPanel, startBurn, bottomBurn, endBurn);
}

float near = 0.52;

Point intToColor(int c) {
  short red = (c << 16) & 0xFF;
  short green = (c << 8) & 0xFF;
  short blue = c & 0xFF;
  return Point(red, green, blue);
}
/*
void manhattanSphereRad(Point *start, Point *color, float tt, int colorI) {
  if (tt < 0) {
    return;
  }

  float t = tt / 3;
  float maxDist = 40;
  float dimness = 0.4;
  long startT = millis();

  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        int xd = x - start->x;
        int yd = y - start->y;
        int zd = z - start->z;
        int dist = abs((xd * xd) + (yd * yd) + (zd * zd) - t * t);
        if (dist < maxDist) {
          float factor = (abs(maxDist - dist) / maxDist);
          float factor1 = factor;
          factor = factor * factor;
          factor = factor * factor1;
          factor = factor * factor;
          cube.setPixel(x, y, z, dimInt(rainbow[colorI], factor * dimness));
        }
      }
    }
  }
}

int colorI = 0;

void sphere() {
  Point start = Point (rand() % xSize, rand() % ySize, rand() % zSize);

  float t = 0.1;
  while(t < 31.3) {
    manhattanSphereRad(&start, &start, t * 2, colorI);
    cube.show();
    colorI = (colorI + 2) % 180;
    long start = millis();
    cube.resetPixels();
    long duration = millis() - start;
    t = pow((pow(t, 1.5) + 2), 2.0/3.0);
  }
  delay(710);
}

void manhattanSphere() {
  Point start = Point (rand() % xSize, rand() % ySize, rand() % zSize);

  for (int t = 0; t < 75; t++) {
      manhattanSphereRad(&start, &start, t * 2, colorI);
      //manhattanSphereRad(&start, &start, t - 50, colorI);
      cube.show();
      //delay(9);
      colorI = (colorI + 2) % 180;
      long start = millis();
      cube.resetPixels();
      long duration = millis() - start;
      Serial.print("reset took: ");
      Serial.println(duration);
    }
  //delay(1000);
}
*/

float breathingFactor = 1;
bool up = true;
int fadeDelayMax = 30;
int fadeDelay = 30;
int fadeIterations = 0;
bool fadeDelayUp = false;

void updateBreathingFactor() {
  if (breathingFactor >= 1.75) {
    up = false;
    Serial.println("top");
  } else if (breathingFactor <= 0.5) {
    Serial.println("bottom");
    up = true; 
  }
  if (up) {
    breathingFactor += 0.003;
  } else {
    breathingFactor -= 0.003;
  }
}

void updateFadeSpeed() {
  fadeIterations++;
  if (fadeIterations == 10) {
    if (fadeDelay >= fadeDelayMax) {
      fadeDelayUp = false;
    } else if (fadeDelay <= 0) {
      fadeDelayUp = true;
    }

    if (fadeDelayUp) {
      fadeDelay++;
    } else {
      fadeDelay--;
    }
  }
}

long curMillis = 0;

void rainbowFade(int colorIndex) {
  updateBreathingFactor();
  //updateFadeSpeed();
  
  curMillis = millis();
  for(int x = 0; x < xSize; x++) {
    for(int y = 0; y < ySize; y++) {
      for(int z = 0; z < zSize; z++) {
        cube.setPixel(x, y, z, dimInt(rainbow[(colorIndex + ((int)((x + y + z) * 3 * breathingFactor))) % 180], 0.17));
        //cube.setPixel(x, y, z, 10, 255, 255);
      }
    }
  }
  cube.show();
  long duration = millis() - curMillis;
  Serial.println(duration);
  //delay(fadeDelay);
  delay(3);
  //delay(20);
}

void std::__throw_out_of_range(char const*) {
  Serial.println("out of range");
}

void std::__throw_length_error(char const*) {
  Serial.println("throwing length error");
}

void std::__detail::_List_node_base::_M_hook(std::__detail::_List_node_base*) {
  Serial.println("ok");
}

void std::__throw_bad_alloc() {
  Serial.println("hi");
}

Point *randomTopPixel() {
  new Point(rand() % xSize, rand() % ySize, zSize - 1);
}

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

RainDrop* randomTopDrop() {
  Point *p = randomTopPixel();
  Point *c = new Point(220 + (rand() % 25), rand() % 20, rand() % 20);
  return new RainDrop(p, c, (rand() % 4) + 2);
}

RainDrop* randomTopDrop(Point* color) {
  Point *p = randomTopPixel();
  return new RainDrop(p, color, (rand() % 4) + 2);
}

void theMatrix(long maxTime) {
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
      cube.setPixel(curPix, cur->color);
      Point* a = curPix->move(PZ);
      cube.setPixel(a, cur->fadedColor);
      a->move_in_place(PZ);
      cube.setPixel(a, cur->moreFadedColor);
      a->move_in_place(PZ);
      cube.setPixel(a, cur->mostFadedColor);
      cur->move();
      if (cube.inCube(cur->point) || cube.inCube(a)) {
        drops.push_back(cur);
      } else {
        delete cur;
      }
      delete a;
    }
    cube.show();
    if (rand() % 4 > 1) {
      int colorIrand = (colorI + rand() % 14) % 180;
      drops.push_back(randomTopDrop(new Point(rainbow[colorIrand])));
      if (rand() % 4 == 0) {
        colorI = (colorI + 1) % 180;
      }
    }
    delay(32);
    cube.resetPixels();
  }


  while (drops.size() > 0) {
    RainDrop* cur = drops.front();
    drops.pop_front();
    delete cur;
  }
}

long loopMillis = 0;
long animationMillis = 0;
long animationDuration = 20000; // 5 minutes
long extraRainbow = 0;

int colorI = 0;

void rainbowFadeWrapper() {
  animationMillis = millis();
  long rainbowDur = animationDuration + extraRainbow;
  while(millis() - animationMillis < rainbowDur) {
    rainbowFade(colorI);
    colorI = (colorI + 1) % 180;
  }
}

void sphereWrapper() {
  animationMillis = millis();
  while(millis() - animationMillis < animationDuration) {
    sphere.sphere();
  }
}

void matrixWrapper() {
  theMatrix(millis() + animationDuration);
}

void loop() {
//  rainbowFadeWrapper();
  sphereWrapper();
//  matrixWrapper();
}

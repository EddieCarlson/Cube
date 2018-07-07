#include <OctoWS2811.h>

#include <Cube.h>
#include <Dumb.h>
#include <Point.h>
#include <rainbow.h>
#include <ExplodingCube.h>
#include <Sphere.h>
#include <RainbowFade.h>
#include <Rain.h>
#include <CubeSlide.h>
#include <ColorWipe.h>

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

Cube *cube = new Cube(leds, xSize, ySize, zSize);

int rainbow[180];

float sphereBrightness = 0.4;
float rainbowBrightness = 0.17;

Sphere sphere(cube, rainbow, sphereBrightness);
RainbowFade rainbowFade(cube, rainbow, rainbowBrightness);
Rain rain(cube, rainbow);
ColorWipe colorWipe(cube, rainbow);

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  delay(1000);
  setupRainbow(rainbow);
  cube->setUp(strandsPerPanel, startBurn, bottomBurn, endBurn);
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

long animationMillis = 0;
long animationDuration = 4 * 60 * 1000; // 4 minutes
long extraTime = 40 * 1000; // 40 seconds

int colorI = 0;

void rainbowFadeWrapper() {
  animationMillis = millis();
  long rainbowDur = animationDuration + extraTime;
  while(millis() - animationMillis < rainbowDur) {
    rainbowFade.rainbowFade();
  }
}


void sphereWrapper() {
  animationMillis = millis();
  while(millis() - animationMillis < animationDuration) {
    sphere.sphere();
  }
}

void rainWrapper() {
  rain.rain(millis() + animationDuration);
}

int cubeSlideMode = 0;

void cubeSlideWrapper() {
  int blockSize;
  int slideSize;
  if (cubeSlideMode == 0) {
    blockSize = 5;
    slideSize = 7;
  } else if (cubeSlideMode == 2) {
    blockSize = 4;
    slideSize = 8;
  } else {
    blockSize = 6;
    slideSize = 6;
  }
  CubeSlide* cubeSlide = new CubeSlide(cube, rainbow, rainbowBrightness, blockSize, slideSize);
  animationMillis = millis();
  long cubeSlideDur = animationDuration - extraTime;
  while(millis() - animationMillis < cubeSlideDur) {
    cubeSlide->move();
  }
  cubeSlideMode = (cubeSlideMode + 1) % 3;
  delete cubeSlide;
}

void colorWipeWrapper() {
  animationMillis = millis();
  long colorWipeDur = (animationDuration - extraTime) - extraTime;
  while(millis() - animationMillis < colorWipeDur) {
    colorWipe.colorWipe();
  }
}

void loop() {
  rainbowFadeWrapper();
  sphereWrapper();
  cubeSlideWrapper();
  rainWrapper();
  colorWipeWrapper();
}

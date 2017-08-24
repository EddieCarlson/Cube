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
CubeSlide *cubeSlide;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);

  delay(1000);
  setupRainbow(rainbow);
  cube->setUp(strandsPerPanel, startBurn, bottomBurn, endBurn);
  cubeSlide = new CubeSlide(cube, rainbow, rainbowBrightness, 5);
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
long animationDuration = 5 * 60 * 1000; // 5 minutes
long extraRainbow = 0;

int colorI = 0;
/*
void rainbowFadeWrapper() {
  animationMillis = millis();
  long rainbowDur = animationDuration + extraRainbow;
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
*/

void cubeSlideWrapper() {
  animationMillis = millis();
  //while(millis() - animationMillis < animationDuration) {
  int count = 0;
  while(true) {
    cubeSlide->move();
    count += 1;
  }
}

void loop() {

//  rainbowFadeWrapper();
//  sphereWrapper();
//  rainWrapper();
  cubeSlideWrapper();
}

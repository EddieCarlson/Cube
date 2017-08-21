#ifndef rainbow_fade_h
#define rainbow_fade_h

class RainbowFade {
  public:
  Cube* cube;
  int colorI;
  int* rainbow;
  int xSize;
  int ySize;
  int zSize;
  float brightness;
  float breathingFactor;
  bool up;
  long curMillis;

  RainbowFade(Cube* _cube, int* _rainbow, float _brightness) {
    cube = _cube;
    colorI = 0;
    rainbow = _rainbow;
    brightness = _brightness;
    breathingFactor = 1;
    up = true;

    xSize = cube->xSize;
    ySize = cube->ySize;
    zSize = cube->zSize;
  }

  void rainbowFade() {
    updateBreathingFactor();
    //updateFadeSpeed();

    curMillis = millis();
    for(int x = 0; x < xSize; x++) {
      for(int y = 0; y < ySize; y++) {
        for(int z = 0; z < zSize; z++) {
          cube->setPixel(x, y, z, dimInt(rainbow[(colorI + ((int)((x + y + z) * 3 * breathingFactor))) % 180], 0.17));
        }
      }
    }
    cube->show();
    long duration = millis() - curMillis;
    colorI = (colorI + 1) % 180;
    delay(3);
  }

  void updateBreathingFactor() {
    if (breathingFactor >= 1.75) {
      up = false;
    } else if (breathingFactor <= 0.5) {
      up = true;
    }
    if (up) {
      breathingFactor += 0.003;
    } else {
      breathingFactor -= 0.003;
    }
  }

  /*
  int fadeDelayMax = 30;
  int fadeDelay = 30;
  int fadeIterations = 0;
  bool fadeDelayUp = false;


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
  */

};

#endif

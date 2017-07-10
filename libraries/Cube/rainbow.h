#ifndef rainbow_h
#define rainbow_h

unsigned int h2rgb(unsigned int v1, unsigned int v2, unsigned int hue);

int makeColor(unsigned int hue, unsigned int saturation, unsigned int lightness);

void setupRainbow(int rainbow[180]);

int dimInt(int i, float f);

#endif

// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// RGBled.h - For control of an RGB LED

#ifndef RGBled_h
#define RGBled_h

#include "Arduino.h"

class RGBled
{
  public:
    RGBled(int redPin, int greenPin, int bluePin);
    void off();
    void mode();
    void cycle();
    void setColorRGB(unsigned int red, unsigned int green, unsigned int blue);
    void color(const bool color[]);
    void flash(const bool color[], int onTime, int offTime);
    void red();
    void green();
    void blue();
    void yellow();
    void cyan();
    void magenta();
    void white();

  private:
    int _pins[3];
    int interval  = 0;
    unsigned int rgbColor[3];
    bool ledState = true;
    unsigned long previousMillis  = 0;
};

#endif

// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// RGBled.cpp - For control af an RGB LED

// ARDUINO LIBRARIES ----------------------------------------------------------
#include "Arduino.h"

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "RGBled.h"

#define ACTIVE LOW // common annode = LOW, common cathode = HIGH

#if ACTIVE == LOW
  #define PWMOFF 255
  #define PWMON  0
#else
  #define PWMOFF 0
  #define PWMON  255
#endif

RGBled::RGBled(int redPin, int greenPin, int bluePin) {
  _pins[0] = redPin;
  _pins[1] = greenPin;
  _pins[2] = bluePin;
}

void RGBled::off() {
  for(int i = 0; i < 3; i++) {
    pinMode(_pins[i], OUTPUT);
    digitalWrite(_pins[i], !ACTIVE);
  }
}

void RGBled::mode() {
  for(int i = 0; i < 3; i++) {
    pinMode(_pins[i], OUTPUT);
  }
}

void RGBled::cycle() {
  rgbColor[0] = PWMON;
  rgbColor[1] = PWMOFF;
  rgbColor[2] = PWMOFF;

  for (int decColor = 0; decColor < 3; decColor += 1) {
    int incColor = decColor == 2 ? 0 : decColor + 1;

    // cross-fade the two colors.
    switch (ACTIVE) {
      case LOW:
        for(int i = 255; i > 0; i -= 1) {
          rgbColor[decColor] += 1;
          rgbColor[incColor] -= 1;

          setColorRGB(rgbColor[2], rgbColor[0], rgbColor[1]);
          delay(2);
        }
        break;
      case HIGH:
        for(int i = 0; i < 255; i += 1) {
          rgbColor[decColor] -= 1;
          rgbColor[incColor] += 1;

          setColorRGB(rgbColor[2], rgbColor[0], rgbColor[1]);
          delay(2);
        }
        break;
    }
  }
}

void RGBled::setColorRGB(unsigned int red, unsigned int green, unsigned int blue) {
  analogWrite(_pins[0], red);
  analogWrite(_pins[1], green);
  analogWrite(_pins[2], blue);
}

void RGBled::color(const bool color[]) {
  mode();
  digitalWrite(_pins[0], !ACTIVE ^ color[0]);
  digitalWrite(_pins[1], !ACTIVE ^ color[1]);
  digitalWrite(_pins[2], !ACTIVE ^ color[2]);
}

void RGBled::flash(const bool value[], int onTime, int offTime) {
  if (ledState) {
    color(value);
  } else {
    off();
  }

  unsigned long currentMillis = millis();

  if ((unsigned long)(currentMillis - previousMillis) >= interval) {
    if (ledState) {
      interval = offTime;
    } else {
      interval = onTime;
    }
    ledState = !ledState;
    previousMillis = currentMillis;
  }
}

void RGBled::red() {
  mode();
  digitalWrite(_pins[0],  ACTIVE);
  digitalWrite(_pins[1], !ACTIVE);
  digitalWrite(_pins[2], !ACTIVE);
}

void RGBled::green() {
  mode();
  digitalWrite(_pins[0], !ACTIVE);
  digitalWrite(_pins[1],  ACTIVE);
  digitalWrite(_pins[2], !ACTIVE);
}

void RGBled::blue() {
  mode();
  digitalWrite(_pins[0], !ACTIVE);
  digitalWrite(_pins[1], !ACTIVE);
  digitalWrite(_pins[2],  ACTIVE);
}

void RGBled::yellow() {
  mode();
  digitalWrite(_pins[0],  ACTIVE);
  digitalWrite(_pins[1],  ACTIVE);
  digitalWrite(_pins[2], !ACTIVE);
}

void RGBled::cyan() {
  mode();
  digitalWrite(_pins[0], !ACTIVE);
  digitalWrite(_pins[1],  ACTIVE);
  digitalWrite(_pins[2],  ACTIVE);
}

void RGBled::magenta() {
  mode();
  digitalWrite(_pins[0],  ACTIVE);
  digitalWrite(_pins[1], !ACTIVE);
  digitalWrite(_pins[2],  ACTIVE);
}

void RGBled::white() {
  mode();
  digitalWrite(_pins[0],  ACTIVE);
  digitalWrite(_pins[1],  ACTIVE);
  digitalWrite(_pins[2],  ACTIVE);
}

void RGBled::fade(const bool value[], int period, int displace) {
  long time = millis();
  int rbval = 128+127*cos(2*PI/period*time);
  int gbval = 128+127*cos(2*PI/period*time);
  int bbval = 128+127*cos(2*PI/period*time);
  analogWrite(_pins[0], rbval);           // sets the value (range from 0 to 255)
  analogWrite(_pins[1], gbval);           // sets the value (range from 0 to 255)
  analogWrite(_pins[2], bbval);           // sets the value (range from 0 to 255)
  if (value[0] == 0) {
    pinMode(_pins[0], OUTPUT);
    digitalWrite(_pins[0], !ACTIVE);
  }
  if (value[1] == 0) {
    pinMode(_pins[1], OUTPUT);
    digitalWrite(_pins[1], !ACTIVE);
  }
  if (value[2] == 0) {
    pinMode(_pins[2], OUTPUT);
    digitalWrite(_pins[2], !ACTIVE);
  }
}

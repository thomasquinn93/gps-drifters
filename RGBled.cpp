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

RGBled::RGBled(int red_pin, int green_pin, int blue_pin) {
  _pins[0] = red_pin;
  _pins[1] = green_pin;
  _pins[2] = blue_pin;
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

// void RGBled::color(const bool color[]) {
//   digitalWrite(_pins[0], !ACTIVE ^ color[0]);
//   digitalWrite(_pins[1], !ACTIVE ^ color[1]);
//   digitalWrite(_pins[2], !ACTIVE ^ color[2]);
// }
//
// void RGBled::flash(const bool value[], int onTime, int offTime) {
//   if (ledState) {
//     color(value);
//   } else {
//     off();
//   }
//
//   unsigned long currentMillis = millis();
//
//   if ((unsigned long)(currentMillis - previousMillis) >= interval) {
//
//     if (ledState) {
//
//       interval = offTime;
//     } else {
//
//       interval = onTime;
//     }
//     ledState = !ledState;
//     previousMillis = currentMillis;
//   }
// }

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

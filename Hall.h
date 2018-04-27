// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Hall.h - For hall effect sensor monitoring

#ifndef Hall_h
#define Hall_h

#include "Arduino.h"

#define ACTIVE LOW // pull up = HIGH, pull down = LOW (CHANGE IN CPP AS WELL)

class Hall
{
  public:
    Hall(int hallPin);
    bool read();
    void readOnce();
    int poll();
  private:
    int _pin;
    bool buttonState;
    bool lastButtonState = !ACTIVE;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;

    unsigned long buttonTimer = 0;
    unsigned long longPressTime = 2000;
    bool buttonActive = false;
    bool longPressActive = false;

    // bool buttonState2;
    // bool lastButtonState2 = !ACTIVE;
};

#endif

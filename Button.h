// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Button.h - For button inputs

#ifndef Button_h
#define Button_h

#include "Arduino.h"

#define ACTIVE HIGH // pull up = HIGH, pull down = LOW (CHANGE IN CPP AS WELL)

class Button
{
  public:
    Button(int buttonPin);
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

    bool buttonState2;
    bool lastButtonState2 = !ACTIVE;
};

#endif

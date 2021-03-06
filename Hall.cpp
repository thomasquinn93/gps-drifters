// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Hall.cpp - For hall effect sensor monitoring

// ARDUINO LIBRARIES ----------------------------------------------------------
#include "Arduino.h"

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "Hall.h"

#define ACTIVE LOW // pull up = HIGH, pull down = LOW (CHANGE IN HEADER ALSO)

Hall::Hall(int hallPin) {
  pinMode(hallPin, INPUT_PULLUP);
  _pin = hallPin;
}

bool Hall::read() {
  bool reading = digitalRead(_pin);

  // If the switch changed, due to noise or pressing reset the debounce timer
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // if buttotn state has been there longer than the debounce delay take it
  // as the actual current state:
  if ((millis() - lastDebounceTime) > debounceDelay) {

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
    }
  }

  lastButtonState = reading;

  return buttonState;
}

int Hall::poll() {
  int clickType = 0;
  if (read() == ACTIVE) {

    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }

    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      clickType = 2;
    }

  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        clickType = 1;
      }
      buttonActive = false;
    }
  }
  return clickType;
}

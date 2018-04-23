// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Battery.cpp - For reading battery information

// ARDUINO LIBRARIES ----------------------------------------------------------
#include "Arduino.h"

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "Battery.h"

#define LOGIC 3.3 // microcontroller logic level
#define MAX   4.2 // battery max voltage
#define MIN   3.2 // battery min voltage

Battery::Battery(int batteryPin, int chargePin) {
  pinMode(batteryPin, INPUT_PULLUP);
  pinMode(chargePin,  INPUT_PULLUP);
  _pins[0] = batteryPin;
  _pins[1] = chargePin;
}

float Battery::read() {
  float batVolt = analogRead(_pins[0]);
  batVolt *= 2;       // potential divider divides by 2, so multiply back
  batVolt *= LOGIC;   // multiply by the reference voltage of 3.3v
  batVolt /= 1024;    // convert to voltage
  return batVolt;
}

int Battery::status() {
  unsigned int state = 0;
  if (read() > MAX*1.04) {
    state = 3;
  } else if (read() < MIN) {
    state = 2;
  } else {
    state = 1;
  }
  return state;
}

bool Battery::charging() {
  bool charging    = false;
  float chargeVolt = analogRead(_pins[1]);

  chargeVolt *= 1.88;    // potential divider divides by factor of 1.88
  chargeVolt *= LOGIC;   // multiply by the reference voltage of 3.3v
  chargeVolt /= 1024;    // convert to voltage

  if (chargeVolt >= 5) {
    charging = true;
  }
  return charging;
}

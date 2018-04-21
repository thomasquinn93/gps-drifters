// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Buzzer.cpp - For control of AC buzzer

// ARDUINO LIBRARIES ----------------------------------------------------------
#include "Arduino.h"

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "Buzzer.h"

#define ACTIVE LOW // micro as sync = LOW, micro as source = HIGH

Buzzer::Buzzer(int pin) {
  _pin = pin;
}

// Turns buzzer off.
void Buzzer::off() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, !ACTIVE);
}

// Turns buzzer on with PWM value of val.
void Buzzer::on(unsigned int val) {
  if (ACTIVE == LOW) {
    val = 255 - val;
  }
  analogWrite(_pin, val);
}

// Plays an arpeggio C,E,G.
void Buzzer::startup() {
  // C5
  tone(_pin, 523, 250);
  delay(200);
  noTone(_pin);

  // E5
  tone(_pin, 659, 250);
  delay(200);
  noTone(_pin);

  // G5
  tone(_pin, 784, 250);
  delay(200);
  noTone(_pin);
}

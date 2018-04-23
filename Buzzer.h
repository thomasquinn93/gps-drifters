// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Button.h - For control of an AC buzzer

#ifndef Buzzer_h
#define Buzzer_h

#include "Arduino.h"

class Buzzer
{
  public:
    Buzzer(int buzzerPin);
    void off();
    void on(unsigned int val);
    void startup();

  private:
    int _pin;
};

#endif

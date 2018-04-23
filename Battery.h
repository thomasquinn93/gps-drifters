// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// Battery.h - For reading battery information

#ifndef Battery_h
#define Battery_h

#include "Arduino.h"

class Battery
{
  public:
    Battery(int batteryPin, int chargePin);
    float read();
    int status();
    bool charging();
  private:
    int _pins[2];

};

#endif

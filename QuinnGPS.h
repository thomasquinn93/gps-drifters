// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// GPS.h - Interfacing between GPS module and microcontroller

#ifndef QuinnGPS_h
#define QuinnGPS_h

#include "Arduino.h"

class QuinnGPS
{
  public:
    QuinnGPS(int rxpin, int txpin);
    void begin(float fs);
    char read();
  private:
    int _pins[3];
};

#endif

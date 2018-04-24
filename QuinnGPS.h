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
    void read();
    uint8_t hour(); //
    uint8_t minute();//
    uint8_t seconds();//
    uint8_t year();//
    uint8_t month();//
    uint8_t day();//
    uint16_t milliseconds();
    float latitude();//
    float longitude();//
    int32_t latitude_fixed();//
    int32_t longitude_fixed();//
    float latitudeDegrees();//
    float longitudeDegrees();//
    float geoidheight();
    float altitude();
    float speed();
    float angle();
    float magvariation();
    float HDOP();
    char lat();
    char lon();
    char mag();
    bool fix();
    uint8_t fixquality();
    uint8_t satellites();
  private:
    int _pins[3];
};

#endif

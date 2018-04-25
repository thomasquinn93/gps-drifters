// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// QuinnSD.h - Interfacing between SD card reader and microcontroller

#ifndef QuinnSD_h
#define QuinnSD_h

#include "Arduino.h"
#include <SD.h>
#include <SPI.h>

class QuinnSD
{
  public:
    QuinnSD(int chipSelect, int cardDetect);
    bool begin();
    void fileOpen();
    void fileName();
    bool fileCreate();
    void fileWrite(char dataBuffer[]);
    void fileClose();
    void writeHeader(uint8_t year, uint8_t month, uint8_t day, float sampleFreq);

  private:
    int _pins[2];
    File file;
    char filename[21];
};

#endif

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
    bool setConfig();
    float getConfig(int n);
    String getValue(String data, char separator, int index);

  private:
    int _pins[2];
    File file;
    File conFile;
    char filename[21];
    char configName[12];
    float config[2];
    char buffer[22];
};

#endif

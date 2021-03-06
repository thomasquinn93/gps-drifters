// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// QuinnSD.cpp - Interfacing between SD card reader and microcontroller

// ARDUINO LIBRARIES ----------------------------------------------------------
#include "Arduino.h"
#include <SD.h>
#include <SPI.h>

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "QuinnSD.h"

QuinnSD::QuinnSD(int chipSelect, int cardDetect) {
  pinMode(cardDetect, INPUT_PULLUP);
  _pins[0] = chipSelect;
  _pins[1] = cardDetect;
}

bool QuinnSD::begin() {
  bool begun = true;
  if (!SD.begin(_pins[0])) {
    begun = false;
  }
  return begun;
}

void QuinnSD::fileOpen() {
  file = SD.open(filename, FILE_WRITE);
}

void QuinnSD::fileName() {
  strcpy(filename, "/DATA/GPSLOG00.CSV");
  for (uint8_t i = 0; i < 100; i++) {
    filename[12] = '0' + i / 10;
    filename[13] = '0' + i % 10;
    if (!SD.exists(filename)) {
      break;
    }
  }
}

bool QuinnSD::fileCreate() {
  bool open = true;
  fileOpen();
  if (!file) {
    open = false;
  }
  return open;
}

void QuinnSD::fileWrite(char dataBuffer[]) {
  fileOpen();
  file.println(dataBuffer);
  fileClose();
}

void QuinnSD::fileClose() {
  file.close();
}

void QuinnSD::writeHeader(uint8_t year, uint8_t month, uint8_t day, float sampleFreq) {
  fileOpen();
  file.print(String(year) + "/" + String(month) + "/" + String(day) + ",");
  file.print("F = ");
  file.print(sampleFreq);
  file.println("Hz");

  file.print("H");
  file.print(",");
  file.print("M");
  file.print(",");
  file.print("S");
  file.print(",");
  file.print("ms");
  file.print(",");

  file.print("Latitude");
  file.print(",");
  file.print("North/South");
  file.print(",");
  file.print("Latitude Fixed");
  file.print(",");
  file.print("Latitude Degrees");
  file.print(",");

  file.print("Longitude");
  file.print(",");
  file.print("East/West");
  file.print(",");
  file.print("Longitude Fixed");
  file.print(",");
  file.print("Longitude Degrees");
  file.print(",");

  file.print("HDOP");
  file.print(",");
  file.print("Geoidal Seperation");
  file.print(",");
  file.print("Magnetic Variation");
  file.print(",");

  file.print("Speed (knots)");
  file.print(",");
  file.print("Angle (Deg)");
  file.print(",");
  file.print("Altitude (M)");
  file.print(",");

  file.print("Quality");
  file.print(",");
  file.print("Satellites");
  file.print(",");
  file.print("Fix");
  file.print(",");
  file.println("Battery (V)");
  fileClose();
}

bool QuinnSD::setConfig() {
  bool exist = true;

  strcpy(configName, "config.txt");
  conFile = SD.open(configName);

  if (!conFile || !SD.exists(configName)) {
    exist = false;
  } else {
    conFile.read(buffer,22);
    String logCharge = getValue(buffer,',',1);
    String sampleFreq = getValue(buffer,',',3);
    config[0] = logCharge.toFloat();
    config[1] = sampleFreq.toFloat();
  }
  conFile.close();
  return exist;
}

float QuinnSD::getConfig(int n) {
  return config[n];
}

String QuinnSD::getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

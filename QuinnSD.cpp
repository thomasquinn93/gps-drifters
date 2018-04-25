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
  file.println("Fix");
  fileClose();
}

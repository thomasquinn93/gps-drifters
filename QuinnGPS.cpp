// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// QuinnGPS.cpp - Interfacing between GPS module and microcontroller

// ARDUINO LIBRARIES ----------------------------------------------------------
#include "Arduino.h"
#include <Adafruit_GPS.h>

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "QuinnGPS.h"

#define GPSSerial Serial1

Adafruit_GPS GPS(&GPSSerial);

QuinnGPS::QuinnGPS(int rxpin, int txpin) {
  _pins[0] = rxpin;
  _pins[1] = txpin;
}

void QuinnGPS::begin(float fs) {
  int sample = fs*10;

  GPS.begin(9600);

  // RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // "minimum recommended" data only
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

  // Set sample rate
  switch (sample) {
    case 2:     // 200mHz, once every 5 seconds
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ);
      break;
    case 1:     // 100mHz, once every 10 seconds
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ);
      break;
    case 20:    // 2Hz
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_2HZ);
      break;
    case 50:    // 5Hz
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);
      break;
    default:    // 1Hz
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
      break;
  }

  // Turn off updates on antenna status, if the firmware permits it
  GPS.sendCommand(PGCMD_NOANTENNA);
}

void QuinnGPS::read() {
  char c = GPS.read();

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // Calling lastNMEA clears received flag and can cause small race conditions
    //    if new data comes in before parse is called again.
    char *stringptr = GPS.lastNMEA();

    // sets the newNMEAreceived() flag to false, if parsing has failed wait
    //    for another
    if (!GPS.parse(stringptr))
      return;

    // if (LOG_FIXONLY && !GPS.fix) {
    //   //Serial.print("No Fix");
    //   return;
    // }

    //Serial.println(stringptr);

  //   uint8_t stringsize = strlen(stringptr);
  //   if (stringsize != logfile.write((uint8_t *)stringptr, stringsize))    //write the string to the SD file
  //      error(4);
  //   if (strstr(stringptr, "RMC") || strstr(stringptr, "GGA"))   logfile.flush();
  //   Serial.println();
  }
}

uint8_t QuinnGPS::hour() {
  uint8_t hour = GPS.hour;
  return hour;
}

uint8_t QuinnGPS::minute() {
  uint8_t minute = GPS.minute;
  return minute;
}

uint8_t QuinnGPS::seconds() {
  uint8_t seconds = GPS.seconds;
  return seconds;
}

uint8_t QuinnGPS::year() {
  uint8_t year = GPS.year;
  return year;
}

uint8_t QuinnGPS::month() {
  uint8_t month = GPS.month;
  return month;
}

uint8_t QuinnGPS::day() {
  uint8_t day = GPS.day;
  return day;
}

uint16_t QuinnGPS::milliseconds() {
  uint8_t milliseconds = GPS.milliseconds;
  return milliseconds;
}

float QuinnGPS::latitude() {
  float latitude = GPS.latitude;
  return latitude;
}

float QuinnGPS::longitude() {
  float longitude = GPS.longitude;
  return longitude;
}

int32_t QuinnGPS::latitude_fixed() {
  int32_t latitude_fixed = GPS.latitude_fixed;
  return latitude_fixed;
}

int32_t QuinnGPS::longitude_fixed() {
  int32_t longitude_fixed = GPS.longitude_fixed;
  return longitude_fixed;
}

float QuinnGPS::latitudeDegrees() {
  float latitudeDegrees = GPS.latitudeDegrees;
  return latitudeDegrees;
}

float QuinnGPS::longitudeDegrees() {
  float longitudeDegrees = GPS.longitudeDegrees;
  return longitudeDegrees;
}

float QuinnGPS::geoidheight() {
  float geoidheight = GPS.geoidheight;
  return geoidheight;
}

float QuinnGPS::altitude() {
  float altitude = GPS.altitude;
  return altitude;
}

float QuinnGPS::speed() {
  float speed = GPS.speed;
  return speed;
}

float QuinnGPS::angle() {
  float angle = GPS.angle;
  return angle;
}

float QuinnGPS::magvariation() {
  float magvariation = GPS.magvariation;
  return magvariation;
}

float QuinnGPS::HDOP() {
  float HDOP = GPS.HDOP;
  return HDOP;
}

char QuinnGPS::lat() {
  char lat = GPS.lat;
  return lat;
}

char QuinnGPS::lon() {
  char lon = GPS.lon;
  return lon;
}

char QuinnGPS::mag() {
  char mag = GPS.mag;
  return mag;
}

bool QuinnGPS::fix() {
  bool fix = GPS.fix;
  return fix;
}

uint8_t QuinnGPS::fixquality() {
  uint8_t fixquality = GPS.fixquality;
  return fixquality;
}

uint8_t QuinnGPS::satellites() {
  uint8_t satellites = GPS.satellites;
  return satellites;
}

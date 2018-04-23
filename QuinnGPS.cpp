// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// GPS.cpp - Interfacing between GPS module and microcontroller

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
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ);
      break;
    case 50:    // 5Hz
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ);
      break;
    case 100:   // 10Hz
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_100_MILLIHERTZ);
      break;
    default:    // 1Hz
      GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
      break;
  }

  // Turn off updates on antenna status, if the firmware permits it
  GPS.sendCommand(PGCMD_NOANTENNA);
}

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
//#define GPSECHO  true
/* set to true to only log to SD when GPS has a fix, for debugging, keep it false */
#define LOG_FIXONLY false

#define chipSelect 4
#define ledPin 13

// read a Hex value and return the decimal equivalent
uint8_t parseHex(char c) {
  if (c < '0')
    return 0;
  if (c <= '9')
    return c - '0';
  if (c < 'A')
    return 0;
  if (c <= 'F')
    return (c - 'A')+10;
}

// blink out an error code
void error(uint8_t errno) {
  /*
  if (SD.errorCode()) {
   putstring("SD error: ");
   Serial.print(card.errorCode(), HEX);
   Serial.print(',');
   Serial.println(card.errorData(), HEX);
   }
   */
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(ledPin, HIGH);
      delay(100);
      digitalWrite(ledPin, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

// void loop() {
//   //if (! usingInterrupt) {
//     // read data from the GPS in the 'main loop'
//     char c = GPS.read();
//     // if you want to debug, this is a good time to do it!
//     //if (GPSECHO)
//       //if (c) Serial.print(c);
//   //}
//
//   // if a sentence is received, we can check the checksum, parse it...
//   if (GPS.newNMEAreceived()) {
//     // a tricky thing here is if we print the NMEA sentence, or data
//     // we end up not listening and catching other sentences!
//     // so be very wary if using OUTPUT_ALLDATA and trying to print out data
//
//     // Don't call lastNMEA more than once between parse calls!  Calling lastNMEA
//     // will clear the received flag and can cause very subtle race conditions if
//     // new data comes in before parse is called again.
//     char *stringptr = GPS.lastNMEA();
//
//     if (!GPS.parse(stringptr))   // this also sets the newNMEAreceived() flag to false
//       return;  // we can fail to parse a sentence in which case we should just wait for another
//
//     // Sentence parsed!
//     //Serial.println("OK");
//     if (LOG_FIXONLY && !GPS.fix) {
//       //Serial.print("No Fix");
//       return;
//     }
//
//     // Rad. lets log it!
//     //Serial.println("Log");
//
//     uint8_t stringsize = strlen(stringptr);
//     if (stringsize != logfile.write((uint8_t *)stringptr, stringsize))    //write the string to the SD file
//         error(4);
//     if (strstr(stringptr, "RMC") || strstr(stringptr, "GGA"))   logfile.flush();
//     //Serial.println();
//   }
// }

char QuinnGPS::read() {
  char c = GPS.read();
  return c;
}

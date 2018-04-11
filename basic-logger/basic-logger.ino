// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// The aim of this script is act as a basic data logger for collecting
//  GPS in a controlled enviroment.

// VERSION HISTORY
// v0.1   - Started basic structure and displayed file name/version on serial
//            port & on OLED.
// v0.2   - Added GPS module but found bug that doesn't update screen.

// HARDWARE (From https://shop.pimoroni.com)
// Adafruit Feather M0 Adalogger     - ADA2796.
// Adafruit FeatherWing - RTC + SD   - ADA2992.
// Adafruit FeatherWing OLED         - ADA2900.

// I/O (Determined via stackable board layout)
// Button B   - Pin 6.
// Battery V  - Pin 9.

#define name "basic-logger.ino"
#define version 0.2
#define BUTTON_B 6
#define debug 0
#define GPSSerial Serial1

// Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h>

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
// Required for Serial on Zero based boards
#define Serial SERIAL_PORT_USBVIRTUAL
#endif

// Objects
Adafruit_SSD1306 display = Adafruit_SSD1306();
Adafruit_GPS GPS(&GPSSerial);

// Pin Variables
uint8_t LED  = 13;

// Global Varaibles
uint8_t menu = 0;
char logg[]  = "LOGGING";
char nlogg[] = "NOT LOGGING";
uint32_t timer = millis();

int gpsHour     = 0;
int gpsMinute   = 0;
int gpsSecond   = 0;
int gpsDay      = 0;
int gpsMonth    = 0;
int gpsYear     = 0;
int gpsFix      = 0;
int gpsQuality  = 0;
int gpsLatitude = 0;
int gpsLat      = 0;
int gpsLongitude= 0;
int gpsLon      = 0;
int gpsSpeed    = 0;
int gpsAngle    = 0;
int gpsAlt      = 0;
int gpsSats     = 0;

void setup() {
  // Serial setup
  Serial.begin(115200);
  if (debug == 1) {
   while(!Serial);
  };
  Serial.println(name);
  Serial.print("v");
  Serial.println(version);

  // GPS setup
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);

  // OLED setup
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(100);
  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(name);
  display.print("v");
  display.println(version);
  display.setCursor(0,0);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();

  // I/O setup
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  delay(100);
}

void loop() {
  gpsData();

  //oledUpdate();

  switch (menu) {
    case 1:
    logging();
    break;
    default:
    idle();
    break;
  }

  if (! digitalRead(BUTTON_B)) {
    menu++;
    delay(200);
  }

  if (menu > 1) {
    menu = 0;
  }
  Serial.print(gpsHour);
  Serial.print(":");
  Serial.print(gpsMinute);
  Serial.print(":");
  Serial.println(gpsSecond);
}
void oledUpdate() {
  // setup display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // print header bar to display
  // print time & date
  display.setCursor(0,0);
  display.print(gpsHour);
  display.print(":");
  display.print(gpsMinute);
  display.print(":");
  display.print(gpsSecond);
  display.setCursor(80,0);
  display.print(gpsDay);
  display.print("/");
  display.print(gpsMonth);
  display.print("/");
  display.print(gpsYear);

  // print footer bar to display
  // print number of satelites avaliable
  display.setCursor(0,25);
  display.print("SATS: ");
  display.print(gpsSats);

  // print menu
  // switch (menu) {
  //   case 1:
  //   // print device mode
  //   display.setCursor(43,12);
  //   display.print(logg);
  //   break;
  //   default:
  //   // print device mode
  //   display.setCursor(32,12);
  //   display.print(nlogg);
  //   break;
  // }

  // print SD capacity
  display.setCursor(80,25);
  display.print("??.?? GB");

  display.display();
  //Serial.println("Display Update");
}
void logging() {
  // print to serial
  //Serial.println(logg);

  // turn on LED
  digitalWrite(LED, HIGH);
}

void idle() {
  // print to serial
  //Serial.println(nlogg);

  // turn off LED
  digitalWrite(LED, LOW);
}

void gpsData() // run over and over again
{
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
    return; // we can fail to parse a sentence in which case we should just wait for another
  }
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
    //Serial.println("GPS Read");
    gpsHour     = GPS.hour;
    gpsMinute   = GPS.minute;
    gpsSecond   = GPS.seconds;
    gpsDay      = GPS.day;
    gpsMonth    = GPS.month;
    gpsYear     = GPS.year;
    gpsFix      = (int)GPS.fix;
    gpsQuality  = (int)GPS.fixquality;
    gpsLatitude = GPS.latitude;
    gpsLat      = GPS.lat;
    gpsLongitude= GPS.longitude;
    gpsLon      = GPS.lon;
    gpsSpeed    = GPS.speed;
    gpsAngle    = GPS.angle;
    gpsAlt      = GPS.altitude;
    gpsSats     = (int)GPS.satellites;
  }
}

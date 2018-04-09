// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// The aim of this script is act as a basic data logger for collecting
//  GPS in a controlled enviroment.

// VERSION HISTORY
// v0.1   - Started basic structure and displayed file name/version on serial
//            port & on OLED.

// HARDWARE (From https://shop.pimoroni.com)
// Adafruit Feather M0 Adalogger     - ADA2796.
// Adafruit FeatherWing - RTC + SD   - ADA2992.
// Adafruit FeatherWing OLED         - ADA2900.

// I/O (Determined via stackable board layout)
// Button B   - Pin 6.
// Battery V  - Pin 9.

#define name "basic-logger.ino"
#define version 0.1
#define BUTTON_B 6
#define debug 1

// Libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif

// Objects
Adafruit_SSD1306 display = Adafruit_SSD1306();

void setup() {
  // Serial setup
  Serial.begin(9600);
  if (debug == 1) {
    while(!Serial);
  };
  Serial.println(name);
  Serial.print("v");
  Serial.println(version);

  // OLED setup
  display.begin();
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
  delay(3000);
  display.clearDisplay();
  display.display();

  // I/O setup
  pinMode(BUTTON_B, INPUT_PULLUP);

  delay(3000);
}

void loop() {

}

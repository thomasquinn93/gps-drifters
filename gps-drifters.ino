// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// GPS Drifter Code

#define VERSION "1.02.00"

// VERSION HISTORY
// v1.00.00  - RGB class written with both analog and digital outs.
//           - Button class written with 3 states, none, short or long press.
//           - Buzzer class written with startup tone.
// v1.01.00  - Added flash function to RGB class.
//           - Idle mode added (ready to log).
//           - Live mode added (print information to serial port).
//           - Added battery class.
// v1.02.00  - Added fade to RGB class.
//           - Added charge detect function to battery class.
//           - RGB fades when charging.
// v1.03.00  - Started GPS class.

// TODO
// - Comment classes.

// HARDWARE (From https://shop.pimoroni.com)
// Adafruit Feather M0 Adalogger     - ADA2796.
// Adafruit FeatherWing - RTC + SD   - ADA2992.
// Adafruit Ultimate GPS FeatherWing - ADA3133.
// C.Anode RGB LED and 3v Buzzer driven with microcontroller as current sync

// I/O (Determined via stackable board layout)
// GPS RX       - Pin 0.
// GPS TX       - Pin 1.
// SD CS        - Pin 4.
// Button       - Pin 5.
// 3V AC Buzzer - Pin 6.
// Card Detect  - Pin 7.
// Battery      - Pin 9 (A7).
// RGB Red      - Pin 10.
// RGB Green    - Pin 11.
// RGB Blue     - Pin 12.

//=============================================================================

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "RGBled.h"
#include "Buzzer.h"
#include "Button.h"
#include "Battery.h"
#include "QuinnGPS.h"

#define ACTIVE LOW
#define DEBUG  false

const int   chipSelect  = 4;
const int   buttonPin   = 5;
const int   buzzerPin   = 6;
const int   cardDetect  = 7;
const int   batteryPin  = A7;
const int   chargePin   = A1;
const int   redPin      = 10;
const int   greenPin    = 11;
const int   bluePin     = 12;

RGBled    RGB(redPin, greenPin, bluePin);
Buzzer    buzzer(buzzerPin);
Button    button(buttonPin);
Battery   battery(batteryPin, chargePin);
QuinnGPS  gps(0,1);

const bool RED[]     = {1, 0, 0};
const bool GREEN[]   = {0, 1, 0};
const bool BLUE[]    = {0, 0, 1};
const bool YELLOW[]  = {1, 1, 0};

unsigned int menu       = 0;
unsigned int submenu    = 0;
const float printFreq   = 1;   // in Hz
const float sampleFreq  = 1;  // in Hz

// SETUP & LOOP ===============================================================

void setup() {
  init_pins();
  start_up();
}

void loop() {
  switch (menu_select(button.poll())) {
    default:  // idle mode
      idle();
      break;
    case 1:   // loggging mode
      RGB.fade(YELLOW, 2000, 500);
      break;
    case 2:   // live mode
      live();
      break;
    }
}

// FUNCTIONS ==================================================================

// Set pinmodes for inputs. RGB pinmodes are set in class due to using both
//    analog & digital out on the same pins.
void init_pins() {
  pinMode(cardDetect, INPUT_PULLUP);
}

// Start serial port, play buzzer startup tone, cycle full RGB spectrum
//    to test LED and begin GPS module.
void start_up() {
  Serial.begin(115200);
  buzzer.startup();
  RGB.off();
  RGB.cycle();
  buzzer.off();
  gps.begin(sampleFreq);
}

// Chooses the menu or submenu based on variable input.
int menu_select(unsigned int buttonState) {
  if (buttonState == 1) {
    menu++;
    if (menu > 2) {
      menu = 0;
    }
  } else if (buttonState == 2) {
    submenu++;
    if (submenu > 1) {
      submenu = 0;
    }
  }
  return menu;
}

// Mode indicating ready to log.
void idle() {
  if (battery.charging() == true) {
    RGB.fade(GREEN, 2000, 500);
  } else {
    RGB.flash(GREEN, 50, 1500);
  }
}

// Mode printing data to serial port at 115200 baud. Prints software version,
//    current millis.
void live() {
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;

  if (battery.charging() == true) {
    RGB.fade(BLUE, 2000, 500);
  } else {
    RGB.flash(BLUE, 50, 1500);
  }

  if (currentMillis - previousMillis >= (1000/printFreq)) {
    previousMillis = currentMillis;
    Serial.print("Version: ");
    Serial.println(VERSION);
    Serial.print("Battery Voltage: " );
    Serial.println(battery.read());
    Serial.print("Battery State: " );
    Serial.println(battery.status());
    Serial.print("Battery Charging: " );
    Serial.println(battery.charging());
    Serial.println(millis());
    Serial.println(digitalRead(cardDetect));
    Serial.println(gps.read());
    Serial.println("");

  }
}

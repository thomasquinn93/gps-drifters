// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// GPS Drifter Code

#define version "1.00.00"

// VERSION HISTORY
// v1.00.00  - RGB class written with both analog and digital outs.
//           - Button class written with 3 states, none, short or long press.
//           - Buzzer class written with startup tone.

// TODO
// - Implement univeral fade in RGB class by saving LED state.
// - Comment classes.
// - Add blinks to LED class. 

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

#define ACTIVE LOW

const int   chipSelect  = 4;
const int   buttonPin   = 5;
const int   buzzerPin   = 6;
const int   cardDetect  = 7;
const float batteryPin  = A7;
const int   redPin      = 10;
const int   greenPin    = 11;
const int   bluePin     = 12;

RGBled  RGB(redPin, greenPin, bluePin);
Buzzer  buzzer(buzzerPin);
Button  button(buttonPin);

unsigned int menu = 0;
unsigned int submenu = 0;

// SETUP & LOOP ===============================================================

void setup() {
  init_pins();
}

void loop() {
  switch (menuSelect(button.poll())) {
    default:  // idle mode
      RGB.green();
      break;
    case 1:   // loggging mode
      RGB.red();
      break;
    case 2:   // live mode
      RGB.blue();
      break;
  }
}

// FUNCTIONS ==================================================================

// Set pinmodes for inputs. RGB pinmodes are set in class due to using both
//    analog & digital out on the same pins. Play buzzer startup tone and cycle
//    full RGB spectrum to test LED.
void init_pins() {
  pinMode(buttonPin,  INPUT_PULLUP);
  pinMode(cardDetect, INPUT_PULLUP);
  buzzer.startup();
  RGB.off();
  RGB.cycle();
  buzzer.off();
}

// Chooses the menu or submenu based on variable input.
int menuSelect(unsigned int buttonState) {
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

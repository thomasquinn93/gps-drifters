// Thomas Quinn B224307 thomas.quinn93@icloud.com
// 17ELD030 - Advanced Project
// GPS Drifter Code

#define VERSION "2.00.02"

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
// v1.04.00  - Finished GPS class.
// v1.05.00  - Wrote SD class.
//           - Data saves to file.
// v1.06.00  - Created Hall Effect sensor class (Same as button class).
//           - Added hall effect sensor to act as button.
// v1.07.00  - Added error states. 1 = White (no gps fix), 2 = Cyan (SD failed
//             to initalise), 3 = Magenta (battery error).
// v1.07.01  - Changed charging constant from 4.6v to 4.4v.
// v1.07.02  - Reverted charging constant to 4.6v.
//           - Changed overcharge voltage from MAX*1.05 to MAX*1.06.
// v1.08.00  - Added battery voltage data saved to SD card.
//           - Suppressed buzzer when logging for battery life testing.
// v1.09.00  - Changed minimum battery voltage to 3.7v before error is
//             displayed due to voltage required for FlashAir SD card.
//           - Changed min charge detect voltage to 4.4V.
//           - Unsuppressed buzzer when logging.
//           - Fixed the white LED no fix bug.
// v2.00.00  - Added feature to load data from config file (log while charging,
//           - and Sample rate).
// v2.00.01  - Reduced charging voltage indicator to 4.25v.
// v2.00.02  - Added extra comments.

// TODO
// - Comment classes.
// - Split file after time period.

// HARDWARE (From https://shop.pimoroni.com)
// Adafruit Feather M0 Adalogger      - ADA2796.
// Adafruit FeatherWing - RTC + SD    - ADA2992.
// Adafruit Ultimate GPS FeatherWing  - ADA3133.

// Other Hardware
// C.Anode RGB LED and 3v Buzzer driven with microcontroller as current sync.
// Pull High Button using 10k pull down resistor.
// RS Components Unipolar Hall Effect - 680-7245.
// Toshiba FlashAir used as SD card.

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
// Hall Effect  - Pin 13.

//=============================================================================

// CUSTOM LIBRARIES -----------------------------------------------------------
#include "RGBled.h"
#include "Buzzer.h"
#include "Button.h"
#include "Battery.h"
#include "QuinnGPS.h"
#include "QuinnSD.h"
#include "Hall.h"

const int chipSelect  = 4;
const int buttonPin   = 5;
const int buzzerPin   = 6;
const int cardDetect  = 7;
const int batteryPin  = A7;
const int chargePin   = A1;
const int redPin      = 10;
const int greenPin    = 11;
const int bluePin     = 12;
const int hallPin     = 13;

RGBled    RGB(redPin, greenPin, bluePin);
Buzzer    buzzer(buzzerPin);
Button    button(buttonPin);
Hall      hall(hallPin);
Battery   battery(batteryPin, chargePin);
QuinnGPS  gps(0,1);
QuinnSD   sd(chipSelect, cardDetect);

const bool RED[]     = {1, 0, 0};
const bool GREEN[]   = {0, 1, 0};
const bool BLUE[]    = {0, 0, 1};
const bool YELLOW[]  = {1, 1, 0};
const bool MAGENTA[] = {1, 0, 1};
const bool CYAN[]    = {0, 1, 1};
const bool WHITE[]   = {1, 1, 1};

unsigned int menu     = 0;
unsigned int submenu  = 0;
float sampleFreq      = 1;     // 0.2, 0.1, 1, 2 or 5 Hz
bool logCharge        = true;  // Log while charging?

// SETUP & LOOP ===============================================================

void setup() {
  start_up();
  checkConfig();
}

void loop() {
  gps.read();
  switch (menu_select(button.poll(), hall.poll())) {
    default:  // idle mode
      idle();
      break;
    case 1:   // loggging mode
      if (battery.charging() && logCharge == false) {
        menu++;
      } else {
        record();
      }
      break;
    case 2:   // live mode
      live();
      break;
    }
}

// FUNCTIONS ==================================================================

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
int menu_select(unsigned int buttonState, unsigned int hallState) {
  if (buttonState == 1) {
    menu++;
    if (menu > 2) {
      menu = 0;
    }
  } else if (buttonState == 2) {
    submenu++;
    if (submenu > 1 || menu != 1) {
      submenu = 0;
    }
  }

  if (hallState == 1) {
    menu++;
    if (menu > 2) {
      menu = 0;
    }
  } else if (hallState == 2) {
    submenu++;
    if (submenu > 1) {
      submenu = 0;
    }
  }


  return menu;
}

// Mode indicating ready to log.
void idle() {
  if (battery.status() != 1) {
    error(3);
  } else {
    if (battery.charging() == true) {
      RGB.fade(GREEN, 2000, 500);
    } else {
      RGB.flash(GREEN, 50, 1500);
    }
  }
}

// Mode printing data to serial port at 115200 baud. Prints software version,
//    current millis and all GPS data.
void live() {
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;

  if (battery.charging() == true) {
    RGB.fade(BLUE, 2000, 500);
  } else {
    RGB.flash(BLUE, 50, 1500);
  }

  gps.read();

  if (currentMillis - previousMillis >= (1000/sampleFreq)) {
    previousMillis = currentMillis;
    Serial.println("========================================================");
    Serial.println("");
    Serial.print("Version: ");            Serial.println(VERSION);
    Serial.print("Card Detect: ");        Serial.println(digitalRead(cardDetect));
    Serial.println("");

    Serial.print("Time: ");               Serial.print(gps.hour());
    Serial.print(":");                    Serial.print(gps.minute());
    Serial.print(":");                    Serial.println(gps.seconds());
    Serial.print("Date: ");               Serial.print(gps.day());
    Serial.print(":");                    Serial.print(gps.month());
    Serial.print(":");                    Serial.println(gps.year());
    Serial.println("");

    Serial.print("Battery Voltage : " );  Serial.println(battery.read());
    Serial.print("Battery State   : " );  Serial.println(battery.status());
    Serial.print("Battery Charging: " );  Serial.println(battery.charging());
    Serial.println("");

    Serial.print("Latitude          : "); Serial.println(gps.latitude());
    Serial.print("Latitude Fixed    : "); Serial.println(gps.latitude_fixed());
    Serial.print("Latitude Degrees  : "); Serial.println(gps.latitudeDegrees());
    Serial.print("Lat : ");               Serial.println(gps.lat());
    Serial.println("");

    Serial.print("Longitude         : "); Serial.println(gps.longitude());
    Serial.print("Longitude Fixed   : "); Serial.println(gps.longitude_fixed());
    Serial.print("Longitude Degrees : "); Serial.println(gps.longitudeDegrees());
    Serial.print("Lon : ");               Serial.println(gps.lon());
    Serial.println("");

    Serial.print("Geoidal Seperation: "); Serial.println(gps.geoidheight());
    Serial.print("Magnetic Variation: "); Serial.println(gps.magvariation());
    Serial.print("Speed (knots): ");      Serial.println(gps.speed());
    Serial.print("Altitude     : ");      Serial.println(gps.altitude());
    Serial.print("Angle        : ");      Serial.println(gps.angle());
    Serial.print("HDOP: ");               Serial.println(gps.HDOP());
    Serial.print("Mag : ");               Serial.println(gps.mag());
    Serial.print("Fix : ");               Serial.println(gps.fix());
    Serial.print("Quality: ");            Serial.println(gps.fixquality());
    Serial.print("Satellites: ");         Serial.println(gps.satellites());
    Serial.println("");

    Serial.print("Millis: ");             Serial.println(millis());
    Serial.println("");
  }
}

// Mode recording data to SD card.
void record() {
  if (battery.charging() == true) {
    RGB.fade(YELLOW, 2000, 500);
  } else {
    RGB.flash(YELLOW, 50, 1500);
  }

  if (submenu == 1) {
    buzzer.on(128);
    for (int i = 0; i < 5; i++) {
      RGB.red();
      delay(100);
      RGB.off();
      delay(100);
    }
    buzzer.off();

    bool sdInserted = sd.begin();

    if (sdInserted == true) {
      sd.fileName();
      if (!sd.fileCreate()) {
        error(2);
      }
      sd.writeHeader(gps.year(), gps.month(), gps.day(), sampleFreq);
    }

    while(1) {
      static int fix = 0;
      unsigned long currentMillis = millis();
      static unsigned long previousMillis = 0;

      if (sdInserted == true) {
        if (fix == 0) {
          error(1);
        } else {
          if (battery.charging() == true) {
            RGB.fade(RED, 2000, 500);
          } else {
            RGB.flash(RED, 50, 1500);
          }
        }

        gps.read();
        buzzer.flash(128, 50, 1500);
        if (currentMillis - previousMillis >= (1000/sampleFreq)) {
          previousMillis = currentMillis;
          fix = gps.fix();
          String data = {String(gps.hour()) + "," + String(gps.minute()) + "," + String(gps.seconds()) + "," + String(gps.milliseconds())
          + "," + String(gps.latitude()) + "," + String(gps.lat()) + "," + String(gps.latitude_fixed()) + "," + String(gps.latitudeDegrees())
          + "," + String(gps.longitude()) + "," + String(gps.lon()) + "," + String(gps.longitude_fixed()) + "," + String(gps.longitudeDegrees())
          + "," + String(gps.HDOP()) + "," + String(gps.geoidheight()) + "," + String(gps.magvariation()) + "," + String(gps.speed())
          + "," + String(gps.angle()) + "," + String(gps.altitude()) + "," + String(gps.fixquality()) + "," + String(gps.satellites()) + "," + String(fix)
          + "," + String(battery.read())};
          char dataBuffer[data.length()+22];
          data.toCharArray(dataBuffer,data.length()+22);

          sd.fileWrite(dataBuffer);

        }
      } else {
        error(2);
      }

      if (button.poll() == 2 || hall.poll() == 2){
        submenu = 0;
        sd.fileClose();
        buzzer.on(128);
        for (int i = 0; i < 5; i++) {
          RGB.yellow();
          delay(100);
          RGB.off();
          delay(100);
        }
        buzzer.off();
        break;
      }
    }
  }
}

// Error code displayed on RGB LED
void error(uint8_t errno) {
  switch (errno) {
    default:
      if (battery.charging() == true) {
        RGB.fade(WHITE, 2000, 500);
      } else {
        RGB.flash(WHITE, 50, 1500);
      }
      break;
    case 2:
      if (battery.charging() == true) {
        RGB.fade(CYAN, 2000, 500);
      } else {
        RGB.flash(CYAN, 50, 1500);
      }
      break;
    case 3:
      RGB.magenta();
      break;
  }
}

// Checks the config file in the root directory of the SD for the sample rate
//    and to see if it should have the option to log data while charging.
void checkConfig() {
  if (sd.begin() && sd.setConfig()){
    logCharge  = (bool) sd.getConfig(0);
    sampleFreq = sd.getConfig(1);
  }
}

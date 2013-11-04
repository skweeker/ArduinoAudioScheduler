/*********************

Example code for the Adafruit RGB Character LCD Shield and Library

This code displays text on the shield, and also reads the buttons on the keypad.
When a button is pressed, the backlight changes color.

**********************/

// include the library code:
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>
#include <RTClib.h>

// The shield uses the I2C SCL and SDA pins. On classic Arduinos
// this is Analog 4 and 5 so you can't use those for analogRead() anymore
// However, you can connect other I2C sensors to the I2C bus and share
// the I2C bus.
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

RTC_DS1307 RTC;
  
// These #defines make it easy to set the backlight color
#define OFF 0x0
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

boolean backlighton;
boolean speaker1;
boolean speaker2;

// LCD icons built at http://mikeyancey.com/hamcalc/lcd_characters.php
// Speaker icon
byte speakerOnChar[8] = {
        B00011,
        B00111,
        B11111,
        B11111,
        B11111,
        B11111,
        B00111,
        B00011
};

byte speakerOffChar[8] = {
        B00011,
        B00101,
        B11001,
        B10001,
        B10001,
        B11001,
        B00101,
        B00011
};

void setup() {
  // Debugging output
  Serial.begin(9600);
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);

  // Start the RTC
  RTC.begin();
  if (! RTC.isrunning()) {
    lcd.setCursor(0, 0);
    lcd.println("RTC is NOT running!");
 
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  // Speaker Chars
  lcd.createChar(0, speakerOnChar);
  lcd.createChar(1, speakerOffChar);
  
  lcd.setBacklight(GREEN);
  backlighton = true;
  
  // Turn on the speakers
  speaker1 = true;
  speaker2 = true;
}

uint8_t i=0;
void loop() {
  // Grab the current time
  DateTime now = RTC.now();
  
  // First row
  lcd.setCursor(3, 0);
  
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  
  lcd.setCursor(4, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  
  // Output the speaker status
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  if (speaker1) {
    lcd.write(0);
  }
  else {
    lcd.write(1);
  }
  if (speaker2) {
    lcd.write(0);
  }
  else {
    lcd.write(1);
  }

  uint8_t buttons = lcd.readButtons();

  if (buttons) {
    if (buttons & BUTTON_LEFT) {
      if (speaker1) {
        speaker1 = false;
      }
      else {
        speaker1 = true;
      }
    }
    
    if (buttons & BUTTON_RIGHT) {
      if (speaker2) {
        speaker2 = false;
      }
      else {
        speaker2 = true;
      }
    }
    
    if (buttons & BUTTON_SELECT) {
      if (backlighton) {
        lcd.setBacklight(OFF);
        backlighton = false;
      }
      else {
        lcd.setBacklight(GREEN);
        backlighton = true;
      }
    }
  }
}

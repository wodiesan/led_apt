/*
Analog RGB LED Stairs Lighting

Checks 3 PIR sensors for motion. Non-addressable (analog) RGB
strip fades-in at a user-selected color.

There are two modes that are governed by an ambient lighting threshold.

Rare Event: Roll for a rare event chance once fade-in is completed.
Upon successful roll, pulse a str message from the RGB strip.

TODO: Separate morse and hue functions into header files.
*/

#include <FastLED.h>
//#include "luxFX.h"
#include "morseFX.h"
//#include "hueFX.h"

// RGB vals for the primary stable color.
#define REDVAL   30
#define GRNVAL   15
#define BLUVAL    0
#define MORSE_CHANCE_D 3000

// LED output and PIR input.
#define REDPIN   10
#define GRNPIN   11
#define BLUPIN    9
#define PIRTOP    6
#define PIRMID    7
#define PIRBOT    8
#define LUXPIN   A0

/*-----------------------------------------------------------
|                      MORSE
-----------------------------------------------------------*/
// Morse payload upon successful roll. 
String morsePayload = " RUN ";

/*-----------------------------------------------------------
|                      PIR
-----------------------------------------------------------*/
// Variables for reading PIR pin status.
int topSen = 0;
int midSen = 0;
int botSen = 0;

/*-----------------------------------------------------------
|                      LUX
-----------------------------------------------------------*/
// Clear day sunset value chosen to test light sensing feature.
float lightLimit = 1;
float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux


// Roll to see if the morse event occurs.
void morseEvent(String morsePayload) {
  long randRoll;
  randRoll = random(MORSE_CHANCE_D);
  
  if (randRoll == MORSE_CHANCE_D - 1) {
    showAnalogRGB(CRGB::Black);
    morseToEmit(morsePayload);
    delay(250);
  }
}


// Takes incoming RGB vals, outputs to 3 PWM pins.
void showAnalogRGB(const CRGB& rgb) {
  analogWrite(REDPIN, rgb.r);
  analogWrite(GRNPIN, rgb.g);
  analogWrite(BLUPIN, rgb.b);
}


// Quick flashes of colors.
void colorBars() {
  showAnalogRGB(CRGB::Green); 
  delay(500);
  showAnalogRGB(CRGB::DarkCyan);  
  delay(500);
  showAnalogRGB(CRGB(75, 30, 0)); // orange
  delay(500);
}

// Check all PIR inputs.
bool checkMotion() {
  if (digitalRead(PIRTOP) || digitalRead(PIRMID) || digitalRead(PIRBOT)) {
    return true;
  } else {
    return false;
  }
}


// Converts str -> ASCII and pulses each element onto the RGB strip.
void morseToEmit(String morseMsg) {
  int charCount;
  int morseChar;
  charCount = morseMsg.length();
  
  for (morseChar = 0; morseChar < charCount; morseChar++) {
    payloadConvert(morseMsg[morseChar]);
  }
}


// Take a single element + emit the morse equivalent.
void payloadConvert(char current) {
  if (current >= 'a' && current <= 'z') {
    flashSequence(MORSE_LETTERS[current - 'a'], current);
  } else if (current >= 'A' && current <= 'Z') {
    flashSequence(MORSE_LETTERS[current - 'A'], current);
  } else if (current >= '0' && current <= '9') {
    flashSequence(MORSE_NUMBERS[current - '0'], current);
  } else if (current == ' ') {
    delay(MORSE_DELAY_SPACE);
  }
}


// Flash a sequence of characters; @param char* sequence; @returns void.
void flashSequence (char* sequence, char current) {
  int i = 0;
  while (sequence[i] != '\0') {
    flashDotOrDash(sequence[i], current);
    i++;
  }
  delay(MORSE_DELAY * 3);
}


// Pulse a single character; @param char; @returns void.
void flashDotOrDash (char unit, char current) {
  
  // Specific colors for ASCII 'R', 'U', and 'N'.
  switch (current) {
    case 82:
      showAnalogRGB(CRGB(0, 180, 0));
      break;
    case 85:
      showAnalogRGB(CRGB(0, 180, 80));
      break;
    case 78:
      showAnalogRGB(CRGB(180, 0, 0));
      break;
    default:
      showAnalogRGB(CRGB(180, 0, 0));
      break;
    break;
  }
  
  
  // Add the correct amount of delay.
  switch (unit) {
    case MORSE_DOT:
      delay(MORSE_DELAY_DOT);
      break;
    case MORSE_DASH:
      delay(MORSE_DELAY_DASH);
      break;
    default:
      break;
    break;
  }
  
  showAnalogRGB(CRGB::Black); 
  delay(MORSE_DELAY); 
}


float RawToLux(int raw)
{
  float logLux = raw * logRange / rawRange;
  return pow(10, logLux);
}


/*-----------------------------------------------------------
|                      SETUP
-----------------------------------------------------------*/
void setup() {
  delay(1000);
  analogReference(EXTERNAL);
  //Serial.begin(9600);
  //Serial.println("Running on 9600 baud.");
  
  // Three color channels for analog strips.
  pinMode(REDPIN, OUTPUT);
  pinMode(GRNPIN, OUTPUT);
  pinMode(BLUPIN, OUTPUT);
  
  // All PIR sensors.
  pinMode(PIRTOP, INPUT);
  pinMode(PIRMID, INPUT);
  pinMode(PIRBOT, INPUT);
  //pinMode(LUXPIN, INPUT);
  
  // Welcome sequence + PIR sensor stabalization time.
  colorBars();
  delay(30000);
}

/*-----------------------------------------------------------
|                      MAIN LOOP
-----------------------------------------------------------*/
void loop() {
  if (checkMotion()) {
    // Read the raw value from the sensor. LEDS on if <= lux threshold.
    int rawValue = analogRead(LUXPIN);
    int luxValue = (RawToLux(rawValue));
    
    // Below ambient lighting threshold.
    if(luxValue <= lightLimit){
      for (int i = 0; i < 16; i++) {
        showAnalogRGB(CRGB(i, 0, 0));
        delay(50);
      }
      
      // Check for morse event. Maintain brightness for a time.
      while (checkMotion()) {
        showAnalogRGB(CRGB(16, 0, 0));
        delay(7000);
      }
      
      // Fade-out RGB strip.
      for (int j = 16; j > 1; j--) {
        showAnalogRGB(CRGB(j, 0, 0));
        delay(50);
      }
    } else {
      // Fade-in RGB strip.
      for (int i = 0; i < 160; i++) {
        showAnalogRGB(CRGB(0, i, i));
        morseEvent(morsePayload);
        delay(25);
      }
      
      // Check for morse event. Maintain brightness for a time.
      while (checkMotion()) {
        //morseEvent(morsePayload);
        showAnalogRGB(CRGB(0, 160, 160));
        delay(7000);
      }
      
      // Fade-out RGB strip.
      for (int j = 160; j > 1; j--) {
        showAnalogRGB(CRGB(0, j, j));
        delay(25);
      }
    }
  } else {
      showAnalogRGB(CRGB::Black);
      delay(200);
  }
}

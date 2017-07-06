/*
Analog RGB LED Stairs Lighting

Checks 3 PIR sensors for motion. Non-addressable (analog) RGB
strip fades-in at a user-selected color.
*/

#include <FastLED.h>

#define REDPIN   10
#define GRNPIN   11
#define BLUPIN    9
#define PIRTOP    8
#define PIRMID    7
#define PIRBOT    6

// variable for reading the pin status
int pirVal = 0;

// Takes incoming RGB vals, outputs to 3 PWM pins.
void showAnalogRGB(const CRGB& rgb) {
  analogWrite(REDPIN, rgb.r);
  analogWrite(GRNPIN, rgb.g);
  analogWrite(BLUPIN, rgb.b);
}


// Quick flashes of colors.
void colorBars() {
  showAnalogRGB(CRGB::DarkGoldenrod);
  delay(500);
  showAnalogRGB(CRGB::Indigo); 
  delay(500);
  showAnalogRGB(CRGB::DarkCyan);  
  delay(500);
  showAnalogRGB(CRGB::Black); 
  delay(500);
}

boolean checkMotion() {
  if (digitalRead(PIRTOP) == HIGH || digitalRead(PIRMID) == HIGH || digitalRead(PIRTOP) == HIGH) {
    return true;
  } else {
    return false;
  }
}

void setup() {
  delay(1000);
  
  // Three color channels for analog strips.
  pinMode(REDPIN, OUTPUT);
  pinMode(GRNPIN, OUTPUT);
  pinMode(BLUPIN, OUTPUT);
  
  // All PIR sensors.
  pinMode(PIRTOP, INPUT);
  pinMode(PIRMID, INPUT);
  pinMode(PIRBOT, INPUT);
  //pinMode(LUXPIN, INPUT);

  // Welcome sequence.
  colorBars();
  // Provide time for the PIR sensors to stabalize.
  delay(30000);
}


void loop() {
  //pirVal = digitalRead(PIRTOP);
  //if (pirVal == HIGH) {
  
  if (checkMotion() == true) {
  //if (digitalRead(PIRTOP) == HIGH || digitalRead(PIRMID) == HIGH || digitalRead(PIRTOP) == HIGH) {
    // Brighten through HSV->RGB conversion.
    for (int i = 0; i < 128; i++) {
      showAnalogRGB( CHSV( i, 100, 255) );
      delay(20); 
    }
    
    // Maintain brightness for 30 sec (+= 3 sec physical delay).
    //while (digitalRead(PIRTOP) == HIGH) {
    while (checkMotion() == true) {
      //showAnalogRGB(CRGB::DarkCyan);
      showAnalogRGB(CRGB::Teal);
      delay(30000);
    }
    
    // Dim through HSV->RGB conversion.
    for (int j = 30; j > 2; j--) {
      showAnalogRGB( CHSV( 50, 100, j ) );
      delay(20); 
    }
    
  } else {
      showAnalogRGB( CRGB::Black );   // turn LED OFF
      delay(200);
  }
}

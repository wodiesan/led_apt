/*
Portland Lighting SE 217: Stairs

Non-functional WIP.
*/

// MORSE_DELAY was originally 200.
//#define MORSE_CHANCE_D 300
#define MORSE_DELAY 100
#define MORSE_DELAY_DOT (MORSE_DELAY * 1)
#define MORSE_DELAY_DASH (MORSE_DELAY * 3)
#define MORSE_DELAY_SPACE (MORSE_DELAY * 7)
#define MORSE_DOT '.'
#define MORSE_DASH '-'


/*-----------------------------------------------------------
|                      MORSE
-----------------------------------------------------------*/
//long randRoll;

// All morse letters; @var *char[]
char* MORSE_LETTERS[] = {
  ".-",     // A
  "-...",   // B
  "-.-.",   // C
  "-..",    // D
  ".",      // E
  "..-.",   // F
  "--.",    // G
  "....",   // H
  "..",     // I
  ".---",   // J
  "-.-",    // K
  ".-..",   // L
  "--",     // M
  "-.",     // N
  "---",    // O
  ".--.",   // P
  "--.-",   // Q
  ".-.",    // R
  "...",    // S
  "-",      // T
  "..-",    // U
  "...-",   // V
  ".--",    // W
  "-..-",   // X
  "-.--",   // Y
  "--.."    // Z
};

// All morse numbers; @var *char[]
char* MORSE_NUMBERS[] = {
  "-----",   // 0
  ".----",   // 1
  "..---",   // 2
  "...--",   // 3
  "....-",   // 4
  ".....",   // 5
  "-....",   // 6
  "--...",   // 7
  "---..",   // 8
  "----."    // 9
};

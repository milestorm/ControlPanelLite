#include <Arduino.h>
#include <config.h>
#include <matrixchars.h>
#include <tones.h>
#include <flasher.h>
#include <OneButton.h>
#include <tools.h>
#include <MaxMatrix.h>
#include <avr/pgmspace.h>
#include <avdweb_VirtualDelay.h>

/*
Control Panel Lite by MileStorm
made in 2019 - 2020

Contains four games:
 - Push the lit button
 - Simon says
 - Sound board
 - <not yet implemented>

uses code for Simon says game from SparkFun Inventor's Kit: Example sketch 16
*/

// helper for array size
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

MaxMatrix dot_matrix(DOTMATRIX_DIN, DOTMATRIX_CS, DOTMATRIX_CLK, DOTMATRIX_DISPLAY_COUNT);

OneButton myButtons[] = {OneButton(BUTTON_RED, true), OneButton(BUTTON_GREEN, true), OneButton(BUTTON_BLUE, true), OneButton(BUTTON_YELLOW, true)};
Flasher myLeds[] = {Flasher(LED_RED, 300, 300), Flasher(LED_GREEN, 300, 300), Flasher(LED_BLUE, 300, 300), Flasher(LED_YELLOW, 300, 300)};

VirtualDelay delay1, delay2, delay3, delay4, delay5, delay6;

bool doSound = false;

// matrix sprites
byte xicht_default[] = {8, 8,
  B00000000,
  B00000110,
  B00010110,
  B00100000,
  B00100000,
  B00010110,
  B00000110,
  B00000000
};

// small score numbers: up to number 99 on display
byte numbers_score[] = {
  B01111100, B01000100, B01111100, B00000000, // 0 = 0
  B00000000, B00001000, B01111100, B00000000, // 1 = 4
  B01110100, B01010100, B01011100, B00000000, // 2 = 8
  B01010100, B01010100, B01111100, B00000000, // 3 = 12
  B00011100, B00010000, B01111100, B00000000, // 4 = 16
  B01011100, B01010100, B01110100, B00000000, // 5 = 20
  B01111100, B01010100, B01110100, B00000000, // 6 = 24
  B00000100, B00000100, B01111100, B00000000, // 7 = 28
  B01111100, B01010100, B01111100, B00000000, // 8 = 32
  B01011100, B01010100, B01111100, B00000000  // 9 = 36
};
// animation definitions
const uint64_t ANIM_lines[] PROGMEM = {
  0x1818181818181818,
  0x0c0c181818183030,
  0x060c0c1818303060,
  0x03060c18183060c0,
  0x0001071e78e08000,
  0x0000033ffcc00000,
  0x000000ffff000000,
  0x0000c0fc3f030000,
  0x0080e0781e070100,
  0x80c060381c060301,
  0x60303018180c0c06,
  0x3030181818180c0c
};
int ANIM_lines_len = sizeof(ANIM_lines)/8;

const uint64_t ANIM_bomb[] PROGMEM = {
  0x0000000000000018,
  0x000000000000183c,
  0x0000000000183c3c,
  0x00000000183c3c3c,
  0x000000183c3c3c3c,
  0x0000183c3c3c3c00,
  0x00183c3c3c3c0000,
  0x183c3c3c3c000000,
  0x3c3c3c3c00000000,
  0x3c7e341c00000000,
  0x3c5ae60001000000,
  0x3c5af62881000000,
  0x1818b166118a4000,
  0x0000099421025024,
  0x000000000050810a,
  0x0000000000000000
};
int ANIM_bomb_len = sizeof(ANIM_bomb)/8;

const uint64_t ANIM_beacon[] PROGMEM = {
  0x7e7e7e7e3c180000,
  0x7c7c7c7c385a8124,
  0x7a7a7a7a345a8124,
  0x7676767634992442,
  0x6e6e6e6e2c992442,
  0x5e5e5e5e2c180000,
  0x3e3e3e3e1c180000
};
int ANIM_beacon_len = sizeof(ANIM_beacon)/8;

const uint64_t ANIM_gun[] PROGMEM = {
  0x00060e167e7d0000,
  0x00060e167e7d0000,
  0x00060e167e7d0000,
  0x00060e167e7e0000,
  0x000103051f1f0000,
  0x0000000107070000,
  0x0000000001010000,
  0x0000000203030000,
  0x0000040205030400,
  0x0008040009010408,
  0x1008000011010000,
  0x0000000021010000,
  0x0000000041010000,
  0x0000000081010000,
  0x0000000001010000,
  0x0000000107070000,
  0x000103051f1f0000
};
int ANIM_gun_len = sizeof(ANIM_gun)/8;

const uint64_t ANIM_laser[] PROGMEM = {
  0x0102028241404080,
  0x01030283c140c080,
  0x01030383c1c0c080,
  0x0102078241e04080,
  0x01020f8241f04080,
  0x01021e8241784080,
  0x01023a82415c4080,
  0x01027282414e4080,
  0x0102e28241474080,
  0x0102c28241434080,
  0x0102828241414080
};
int ANIM_laser_len = sizeof(ANIM_laser)/8;

const uint64_t ANIM_pulsating[] PROGMEM = {
  0x8100000000000081,
  0x8142000000004281,
  0x0042240000244200,
  0x0000241818240000,
  0x0000001818000000,
  0x0000183c3c180000,
  0x0018244242241800,
  0x1842008181004218
};
int ANIM_pulsating_len = sizeof(ANIM_pulsating)/8;

const uint64_t ANIM_fx1[] PROGMEM = {
  0x0101010101010101,
  0x0103030303030301,
  0x0103070707070301,
  0x0103070f0f070301,
  0x0002061e1e060200,
  0x0000243c3c240000,
  0x0040607878604000,
  0x80c0e0f0f0e0c080,
  0x80c0e0e0e0e0c080,
  0x80c0c0c0c0c0c080,
  0x8080808080808080,
  0x4040808080808080,
  0x0020204040408080,
  0x0010102020404080,
  0x0000081020204080,
  0x0000040810204080,
  0x0000000408304080,
  0x0000000006186080,
  0x00000000000638c0,
  0x00000000000003fc,
  0x00000000000000ff,
  0x00000000000081ff,
  0x000000000081c3ff,
  0x0000000081c3e7ff,
  0x00000081c3e7ff7e,
  0x000081c3e7ff7e3c,
  0x0081c3e7ff7e3c18,
  0x81c3e7ff7e3c1800,
  0xc3e7ff7e3c180000,
  0xe7ff7e3c18000000,
  0xff7e3c1800000000,
  0x7e3c180000000000,
  0x3c18000000000000,
  0x1800000000000000,
  0x0000000000000000
};
int ANIM_fx1_len = sizeof(ANIM_fx1)/8;

const uint64_t ANIM_fx2[] PROGMEM = {
  0x8142241818244281,
  0x4021121c38488402,
  0x2010111e78880804,
  0x1010101ff8080808,
  0x080808f81f101010,
  0x040888781e111020,
  0x028448381c122140
};
int ANIM_fx2_len = sizeof(ANIM_fx2)/8;


const uint64_t ANIM_FACE_lol[] PROGMEM = {
  0x00003c4200666600,
  0x003c7e4200660000,
  0x3c427e4200006600,
  0x003c427e42006600,
  0x3c427e4200660000,
  0x003c427e42006600,
  0x3c427e4200660000,
  0x003c427e42006600,
  0x3c427e4200006600,
  0x3c427e4200666600,
  0x003c7e4200666600
};
int ANIM_FACE_lol_length = sizeof(ANIM_FACE_lol)/8;

const uint64_t ANIM_FACE_sad[] PROGMEM = {
  0x00003c0000666600,
  0x0018241800666600,
  0x0018241800666600,
  0x0018241800666600,
  0x0018241800666600,
  0x0018241800660000,
  0x00423c0066000000,
  0x00423c0066000000,
  0x00423c0066000000,
  0x00423c0066000000,
  0x0000423c00660000,
  0x00423c0066000000,
  0x00423c0066000000,
  0x00423c0066000000,
  0x00423c0066000000
};
int ANIM_FACE_sad_length = sizeof(ANIM_FACE_sad)/8;



// Scrolling text
byte buffer[10];
char start_message_easybuttons[] =  " Easy Buttons   ";
char start_message_timedbuttons[] =    " Timed Buttons   ";
char start_message_simon[] =    " Simon says   ";
char start_message_soundboard[] =    " Soundboard   ";

// gameType: menu -1, easybutton 0, simon 1, soundboard 2,  timedbutton 3
int gameType = -1;

// ----------------------------------------------------------------
// misc functions

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      dot_matrix.setDot(j, i, bitRead(row, j));
    }
  }
}

void runAnimation(const uint64_t *animArray, const int animLength, int cyclesCount = 1, int frameDelay = 100){
  uint64_t image;
  for (int j = 0; j < cyclesCount; j++){
    for (int i = 0; i < animLength; i++){
      memcpy_P(&image, &animArray[i], 8);
      displayImage(image);
      delay(frameDelay); // predelat na nonblocking delay
    }
  }
}

// non blocking animation class
class DotMatrixAnimation {
  bool isAnimating = false;

  const uint64_t *animArray;
  int animLength;
  int cyclesCount;
  int frameDelay;

  bool isInfinite = false;

  // counter helpers
  int cyclesIndex = 0;
  int animIndex = 0;

  VirtualDelay animDelay; // initialize virtual delay

  private:
  void displayImage(uint64_t image) {
    for (int i = 0; i < 8; i++) {
      byte row = (image >> i * 8) & 0xFF;
      for (int j = 0; j < 8; j++) {
        dot_matrix.setDot(j, i, bitRead(row, j));
      }
    }
  }

  public:
  DotMatrixAnimation(const uint64_t *_animArray, int _animLength, int _frameDelay = 100){
    animArray = _animArray;
    animLength = _animLength;
    frameDelay = _frameDelay;
  }

  bool isRunning() {
    return isAnimating;
  }

  void setInfinite(bool value = true) {
    isInfinite = value;
  }

  void start(int _cyclesCount = 1) {
    cyclesCount = _cyclesCount;
    isAnimating = true;
  }

  void stop() {
    isAnimating = false;
  }

  void tick() {
    if (isAnimating == true) {
      animDelay.start(frameDelay);
      if (animDelay.elapsed()) {

        uint64_t image;

        memcpy_P(&image, &animArray[this->animIndex], 8);
        displayImage(image);

        if (animIndex < animLength - 1) {
          animIndex++;
        } else {
          if (isInfinite) {
            animIndex = 0;
          } else {
            if (cyclesIndex == cyclesCount - 1) {
              isAnimating = false;
            }
            animIndex = 0;
            cyclesIndex++;
          }

        }

      }
    }
  }



};

DotMatrixAnimation animation_intro(ANIM_fx2, ANIM_fx2_len);

/* zvuky udelat neco na zpusob rtttl
zapsany prikazy v array

tone: frequency, tonelength (ms)
  T:440;1000
pause: pauselength
  P:500
sweep: startfreq, stopfreq, step, tonelength
  S:1000;2000;10;5
random: minfreq maxfreq mintonelength maxtonelength
  R:300;500;2;8

example
  {"T:800;100", "P:50", "T:1000;100", "P:50", "T:1200;100", "P:50", "S:2200;1000;10;5"}


*/

class ToneSfx {
  int pin;
  char **inputArray;
  int inputArrayLength;
  int arrayIndex = 0;

  bool playing = false;
  bool infinitePlayback = false;

  bool readCommand = false;
  int commandType = -1;
  char *readValue;

  int num;
  int frequency;
  int duration;

  VirtualDelay soundDelay;

  public:
  ToneSfx(int _pin, char **_inputArray, int _inputArrayLength) {
    inputArray = _inputArray;
    pin = _pin;
    //inputArrayLength = sizeof(inputArray) / sizeof(inputArray[0]);
    inputArrayLength = _inputArrayLength;
  }

  bool isPlaying() {
    return playing;
  }

  void setInfinite(bool value = true) {
    infinitePlayback = value;
  }

  void start() {
    playing = true;
  }

  void stop() {
    playing = false;
  }

  void tick() {
    if (playing) {
      // reads value from array once
      if (!readCommand) {
        readValue = inputArray[arrayIndex];
        readCommand = true;
        Serial.print("^ READING COMMAND: ");
        Serial.println(readValue);
      }

      // Prepares values for effects
      // TONE
      if (*readValue == 'T') {
        Serial.println("TONE");

        readValue++; readValue++; // skip T:
        num = 0;
        while(isdigit(*readValue)) {
          num = (num * 10) + (*readValue++ - '0');
        }
        frequency = num;
        readValue++; // skip semicolon

        num = 0;
        while(isdigit(*readValue)) {
          num = (num * 10) + (*readValue++ - '0');
        }
        duration = num;
        commandType = 0;
      }
      // PAUSE
      else if (*readValue == 'P') {
        Serial.println("PAUSE");
        readCommand = false;
      }
      // SWEEP
      else if (*readValue == 'S') {
        Serial.println("SWEEP");
        readCommand = false;
      }
      // RANDOM
      else if (*readValue == 'R') {
        Serial.println("RANDOM");
        readCommand = false;
      }

      // tone generation
      switch (commandType) {
      case 0: // TONE
          tone(pin, frequency, duration);
          soundDelay.start(duration);
          if(soundDelay.elapsed()) {
            readCommand = false;
            commandType = -1;
          }
        break;

      case 1: // PAUSE
        break;

      case 2: // SWEEP
        break;

      case 3: // RANDOM
        break;

      default:
        break;
      }

      if (!readCommand) {
        Serial.println(">> NEXT COMMAND");
        if (arrayIndex < inputArrayLength - 1) {
          arrayIndex++;
        } else {
          if (infinitePlayback) {
            arrayIndex = 0;
          } else {
            arrayIndex = 0;
            playing = false;
          }
        }
      }

    }
  }

};

char *examplestring[] = {"T:800;100", "P:50", "T:1000;100", "P:50", "T:1200;100", "P:50", "S:2200;1000;10;5"};


ToneSfx tonn(BUZZER, examplestring, sizeof(examplestring) / sizeof(examplestring[0]));


void sound_fx1_update() {
  if (doSound == true) {
    tone(BUZZER, 800, 100);
    delay(150);
    tone(BUZZER, 1000, 100);
    delay(150);
    tone(BUZZER, 1200, 100);
    delay(150);
    for (int i = 2200 - 1; i >= 1000; i=i-10){
      tone(BUZZER, i, 5);
      delay(5);
    }
  }
}

// --------------------------------------------------------------------------------

// Button game
// ===========

int activeLed = -1;
int buttonGameScore = 0;

void turnOnAllLeds() {
	for (int i = 0; i < 4; i++) {
		myLeds[i].permanentOn();
	}
}

void turnOffAllLeds() {
	for (int i = 0; i < 4; i++) {
		myLeds[i].off();
	}
}

void flashAllLeds() {
	for (int i = 0; i < 4; i++) {
		myLeds[i].flashingOn();
	}
}

int turnOnRandomLed() {
	int rndNumber;

	// makes the next led different than the one, that is lit.
	// do {
	// 	rndNumber = randomGenerator(0,3);
	// } while (rndNumber == activeLed);

  rndNumber = randomGenerator(0,3);

	myLeds[rndNumber].permanentOn();
	return rndNumber;
}

// ----------------------------------------------------------------
// Simon says
// ==========

// Game state variables
byte gameBoard[32]; //Contains the combination of buttons as we advance
byte gameRound = 0; //Counts the number of succesful rounds the player has made it through

// Put extracted character on Display
void printCharWithShift(char c, int shift_speed){
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7 * c, 7);
  dot_matrix.writeSprite(DOTMATRIX_DISPLAY_COUNT * 8, 0, buffer);
  dot_matrix.setColumn(DOTMATRIX_DISPLAY_COUNT * 8 + buffer[0], 0);

  for (int i = 0; i < buffer[0] + 1; i++)
  {
    delay(shift_speed);
    dot_matrix.shiftLeft(false, false);
  }
}

// Extract characters from Scrolling text
void printStringWithShift(char* s, int shift_speed){
  while (*s != 0){
    printCharWithShift(*s, shift_speed);
    s++;
  }
}

void writeScore(int scoreInput, bool scrollable = true) {
  byte finalSprite[10];
  int spritePos = 4;
  finalSprite[1] = 8;
  int ones;
  int tens;

  if (scoreInput < 10) { // jednoznakovy cisla
    finalSprite[0] = 4;
    spritePos = 4;
    for (int i = 2; i < 6; i++) { //6,10
        finalSprite[i] = numbers_score[(scoreInput * 4) + (i - 2)];
    }
  }
  else { // dvouznakovy cisla
    finalSprite[0] = 8;
    spritePos = 0;
    ones=scoreInput%10;
    scoreInput=scoreInput/10;
    tens=scoreInput%10;

    for (int i = 2; i < 6; i++) {
        finalSprite[i] = numbers_score[(tens * 4) + (i - 2)];
    }
    for (int i = 6; i < 10; i++) {
        finalSprite[i] = numbers_score[(ones * 4) + (i - 6)];
    }

  }
  if (scrollable == true){
    dot_matrix.writeSprite(spritePos+8, 0, finalSprite);
    for (int i=0; i<8; i++){
      dot_matrix.shiftLeft(false, false);
      delay(100);
    }
  } else {
    dot_matrix.writeSprite(spritePos, 0, finalSprite);
  }

}

// Adds a new random button to the game sequence, by sampling the timer
void add_to_moves(void)
{
  byte newButton = randomGenerator(0, 4); //min (included), max (exluded)

  // We have to convert this number, 0 to 3, to CHOICEs
  if(newButton == 0) newButton = CHOICE_RED;
  else if(newButton == 1) newButton = CHOICE_GREEN;
  else if(newButton == 2) newButton = CHOICE_BLUE;
  else if(newButton == 3) newButton = CHOICE_YELLOW;

  gameBoard[gameRound++] = newButton; // Add this new button to the game array
}

// Toggle buzzer every buzz_delay_us, for a duration of buzz_length_ms.
void buzz_sound(int buzz_length_ms, int buzz_delay_us)
{
  // Convert total play time from milliseconds to microseconds
  long buzz_length_us = buzz_length_ms * (long)1000;

  // Loop until the remaining play time is less than a single buzz_delay_us
  while (buzz_length_us > (buzz_delay_us * 2))
  {
    buzz_length_us -= buzz_delay_us * 2; //Decrease the remaining play time

    // Toggle the buzzer at various speeds
    digitalWrite(BUZZER, HIGH);
    delayMicroseconds(buzz_delay_us);

    digitalWrite(BUZZER, LOW);
    delayMicroseconds(buzz_delay_us);
  }
}

// Lights a given LEDs
// Pass in a byte that is made up from CHOICE_RED, CHOICE_YELLOW, etc
void setLEDs(byte leds)
{
  if ((leds & CHOICE_RED) != 0)
    digitalWrite(LED_RED, HIGH);
  else
    digitalWrite(LED_RED, LOW);

  if ((leds & CHOICE_GREEN) != 0)
    digitalWrite(LED_GREEN, HIGH);
  else
    digitalWrite(LED_GREEN, LOW);

  if ((leds & CHOICE_BLUE) != 0)
    digitalWrite(LED_BLUE, HIGH);
  else
    digitalWrite(LED_BLUE, LOW);

  if ((leds & CHOICE_YELLOW) != 0)
    digitalWrite(LED_YELLOW, HIGH);
  else
    digitalWrite(LED_YELLOW, LOW);
}

// Light an LED and play tone
// Red, upper left:     440Hz - 2.272ms - 1.136ms pulse
// Green, upper right:  880Hz - 1.136ms - 0.568ms pulse
// Blue, lower left:    587.33Hz - 1.702ms - 0.851ms pulse
// Yellow, lower right: 784Hz - 1.276ms - 0.638ms pulse
void toner(byte which, int buzz_length_ms)
{
  setLEDs(which); //Turn on a given LED

  //Play the sound associated with the given LED
  switch(which)
  {
  case CHOICE_RED:
    buzz_sound(buzz_length_ms, 1136);
    break;
  case CHOICE_GREEN:
    buzz_sound(buzz_length_ms, 568);
    break;
  case CHOICE_BLUE:
    buzz_sound(buzz_length_ms, 851);
    break;
  case CHOICE_YELLOW:
    buzz_sound(buzz_length_ms, 638);
    break;
  }

  setLEDs(CHOICE_OFF); // Turn off all LEDs
}

// Plays the current contents of the game moves
void playMoves(void)
{
  for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++) 
  {
    toner(gameBoard[currentMove], 150);

    // Wait some amount of time between button playback
    // Shorten this to make game harder
    delay(150); // 150 works well. 75 gets fast.
  }
}

// Returns a '1' bit in the position corresponding to CHOICE_RED, CHOICE_GREEN, etc.
byte checkButton(void)
{
  if (digitalRead(BUTTON_RED) == 0) return(CHOICE_RED);
  else if (digitalRead(BUTTON_GREEN) == 0) return(CHOICE_GREEN);
  else if (digitalRead(BUTTON_BLUE) == 0) return(CHOICE_BLUE);
  else if (digitalRead(BUTTON_YELLOW) == 0) return(CHOICE_YELLOW);

  return(CHOICE_NONE); // If no button is pressed, return none
}

// Wait for a button to be pressed.
// Returns one of LED colors (LED_RED, etc.) if successful, 0 if timed out
byte wait_for_button(void)
{
  long startTime = millis(); // Remember the time we started the this loop

  while ( (millis() - startTime) < ENTRY_TIME_LIMIT) // Loop until too much time has passed
  {
    byte button = checkButton();

    if (button != CHOICE_NONE)
    {
      toner(button, 150); // Play the button the user just pressed

      while(checkButton() != CHOICE_NONE) ;  // Now let's wait for user to release button

      delay(10); // This helps with debouncing and accidental double taps

      return button;
    }

  }

  return CHOICE_NONE; // If we get here, we've timed out!
}

// Play the regular memory game
// Returns 0 if player loses, or 1 if player wins
boolean play_memory(void)
{
  randomSeed(millis()); // Seed the random generator with random amount of millis()

  gameRound = 0; // Reset the game to the beginning

  while (gameRound < ROUNDS_TO_WIN)
  {
    add_to_moves(); // Add a button to the current moves, then play them back

    playMoves(); // Play back the current game board

    // Then require the player to repeat the sequence.
    for (byte currentMove = 0 ; currentMove < gameRound ; currentMove++)
    {
      byte choice = wait_for_button(); // See what button the user presses

      if (choice == 0) return false; // If wait timed out, player loses

      if (choice != gameBoard[currentMove]) return false; // If the choice is incorect, player loses
    }

    // Player was correct, delay before playing moves
    runAnimation(ANIM_FACE_lol, ANIM_FACE_lol_length);
    dot_matrix.writeSprite(0, 0, xicht_default);
  }

  return true; // Player made it through all the rounds to win!
}

// Play the winner sound
// This is just a unique (annoying) sound we came up with, there is no magic to it
void winner_sound(void)
{
  // Toggle the buzzer at various speeds
  for (byte x = 250 ; x > 70 ; x--)
  {
    for (byte y = 0 ; y < 3 ; y++)
    {
      digitalWrite(BUZZER, HIGH);
      delayMicroseconds(x);

      digitalWrite(BUZZER, LOW);
      delayMicroseconds(x);
    }
  }
}

// Play the winner sound and lights
void play_winner(void)
{
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  winner_sound();
  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  winner_sound();
  setLEDs(CHOICE_GREEN | CHOICE_BLUE);
  winner_sound();
  setLEDs(CHOICE_RED | CHOICE_YELLOW);
  winner_sound();
}

void play_intro(void)
{
  printStringWithShift(start_message_simon, 100);  // Send scrolling Text

  play_winner();
}

// Play the loser sound/lights
void play_loser(void)
{
  setLEDs(CHOICE_RED | CHOICE_GREEN);
  buzz_sound(255, 1500);

  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
  buzz_sound(255, 1500);

  setLEDs(CHOICE_RED | CHOICE_GREEN);
  buzz_sound(255, 1500);

  setLEDs(CHOICE_BLUE | CHOICE_YELLOW);
  buzz_sound(255, 1500);
}

// Show an "attract mode" display while waiting for user to press button.
void attractMode(void)
{
  while(1)
  {
    setLEDs(CHOICE_RED);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_BLUE);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_GREEN);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;

    setLEDs(CHOICE_YELLOW);
    delay(100);
    if (checkButton() != CHOICE_NONE) return;
  }
}

// ----------------------------------------------------------------

// button handler

void processPush(int buttonId) {
  Serial.print("ButtonID: ");
  Serial.println(buttonId);
  Serial.print("GameType: ");
  Serial.println(gameType);

  switch (gameType){
  case -1:
    // main menu
    gameType = buttonId;

    // inits of games
    switch (buttonId){
    case 0:
      // easy buttons game
      animation_intro.stop();
      turnOffAllLeds();
      printStringWithShift(start_message_easybuttons, 100);
      writeScore(buttonGameScore, false);
      activeLed = turnOnRandomLed();
      break;

    case 1:
      // simon
      animation_intro.stop();
      turnOffAllLeds();
      play_intro();
      break;

    case 2:
      // sound board
      //animation_intro.stop();
      turnOffAllLeds();
      printStringWithShift(start_message_soundboard, 100);
      break;

    case 3:
      // timed buttons game
      animation_intro.stop();
      turnOffAllLeds();
      printStringWithShift(start_message_timedbuttons, 100);
      break;

    default:
      break;
    }
    break;

  // every other push in game
  case 0:
    // easy buttons game
    if (myLeds[buttonId].isPermanentOn()) {
      turnOffAllLeds();
      activeLed = turnOnRandomLed();
      buttonGameScore += 1;
      tone(BUZZER, NOTE_A5, 100);
      delay(100);
      tone(BUZZER, NOTE_C6, 200);
    } else {
      myLeds[activeLed].flashingOn();
      buttonGameScore = 0;
      tone(BUZZER, NOTE_A3, 400);
      delay(400);
      tone(BUZZER, NOTE_G3, 400);
      delay(400);
      tone(BUZZER, NOTE_C3, 400);
      delay(400);
      dot_matrix.clear();
    }
    writeScore(buttonGameScore, false);
    break;

  case 1:
    // simon
    // processes in the loop section, not here
    break;

  case 2:
    // sound board
    tonn.start();
    break;

  case 3:
    // timed buttons game
    runAnimation(ANIM_fx1, ANIM_fx1_len, 1, 50);
    runAnimation(ANIM_FACE_sad, ANIM_FACE_sad_length);
    runAnimation(ANIM_FACE_lol, ANIM_FACE_lol_length);
    runAnimation(ANIM_gun, ANIM_gun_len, 2);
    runAnimation(ANIM_laser, ANIM_laser_len);
    runAnimation(ANIM_lines, ANIM_lines_len);
    runAnimation(ANIM_pulsating, ANIM_pulsating_len, 3);
  break;

  default:
    break;
  }

}

// RED
void butt0Click() {
	processPush(0);
}

// GREEN
void butt1Click() {
	processPush(1);
}

// BLUE
void butt2Click() {
	processPush(2);
}

// YELLOW
void butt3Click() {
	processPush(3);
}

// -----------------------------------------
// SETUP
// ------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");

  pinMode(BUZZER, OUTPUT);

  dot_matrix.init(); // module initialize
  dot_matrix.setIntensity(3); // dot matix intensity 0-15
  // bootup
  turnOnAllLeds();

  animation_intro.setInfinite();
  animation_intro.start();
  tone(BUZZER, NOTE_E5, 100);
  delay(100);
  tone(BUZZER, NOTE_E6, 400);
  flashAllLeds();

	for (int i = 0; i < 4; i++){
		myButtons[i].setPressTicks(60);
	}

	myButtons[0].attachPress(butt0Click);
	myButtons[1].attachPress(butt1Click);
	myButtons[2].attachPress(butt2Click);
	myButtons[3].attachPress(butt3Click);

  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
}

// --------------------------------------
// LOOP
// --------------------------------------

void loop() {
  switch (gameType){
  case 1:
    // loop for simon
    attractMode(); // Blink lights while waiting for user to press a button

    // Indicate the start of game play
    dot_matrix.clear();
    dot_matrix.writeSprite(0, 0, xicht_default);
    setLEDs(CHOICE_RED | CHOICE_GREEN | CHOICE_BLUE | CHOICE_YELLOW); // Turn all LEDs on
    delay(1000);
    setLEDs(CHOICE_OFF); // Turn off LEDs
    delay(250);


    // Play memory game and handle result
    if (play_memory() == true) {
      play_winner(); // Player won, play winner tones
      dot_matrix.clear();
      writeScore(gameRound-1);
    }
    else {
      play_loser(); // Player lost, play loser tones
      runAnimation(ANIM_FACE_sad, ANIM_FACE_sad_length);
      dot_matrix.clear();
      writeScore(gameRound-1);
    }
    break;

  default:
    // loop for everything else
    for (int i = 0; i < 4; i++){
      myButtons[i].tick();
      myLeds[i].tick();
    }

    animation_intro.tick();
    tonn.tick();
    break;
  }
}

// TODO:
/*
u soundboardu udelat naky animace s virtualdelay
array framu, ktery se zobrazej spolu se zvukem, kterej bude taky pres virtual delay
efektu a animaci by bylo 8, protoze press a longpress
*/
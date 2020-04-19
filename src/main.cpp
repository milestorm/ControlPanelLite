#include <Arduino.h>
#include <config.h>
#include <flasher.h>
#include <OneButton.h>
#include <tools.h>
#include <MaxMatrix.h>

MaxMatrix dotMatrix(MATRIX_DIN, MATRIX_CS, MATRIX_CLK, 1);

OneButton myButtons[] = {OneButton(BUTTON_R, true), OneButton(BUTTON_G, true), OneButton(BUTTON_B, true), OneButton(BUTTON_Y, true)};
Flasher myLeds[] = {Flasher(LIGHT_R, 300, 300), Flasher(LIGHT_G, 300, 300), Flasher(LIGHT_B, 300, 300), Flasher(LIGHT_Y, 300, 300)};

byte xicht_happy[] =   {8, 8, B00000000, B00100000, B01001100, B01000000, B01000000, B01001100, B00100000, B00000000};
byte xicht_wink[] =    {8, 8, B00000000, B00100100, B01000100, B01000000, B01000000, B01001100, B00100000, B00000000};
byte xicht_sad[] =     {8, 8, B00000000, B01000000, B00101100, B00100000, B00100000, B00101100, B01000000, B00000000};
byte xicht_default[] = {8, 8, B00000000, B00000000, B00101100, B00100000, B00100000, B00101100, B00000000, B00000000};

int activeLed = -1;

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

int turnOnRandomLed() {
	int rndNumber;

	// makes the next led different than the one, that is lit.
	do {
		rndNumber = randomGenerator(0,3);
	} while (rndNumber == activeLed);

	myLeds[rndNumber].permanentOn();
	return rndNumber;
}

void processPush(int buttonId) {
	if (myLeds[buttonId].isPermanentOn()) {
		turnOffAllLeds();
		activeLed = turnOnRandomLed();
    tone(SPEAKER, 880, 100);
	} else {
		myLeds[activeLed].flashingOn();
    tone(SPEAKER, 220, 100);
	}
}

void butt0Click() {
	processPush(0);
}

void butt1Click() {
	processPush(1);
}

void butt2Click() {
	processPush(2);
}

void butt3Click() {
	processPush(3);
}

// -----------------------------------------

void setup() {
  dotMatrix.init(); // module initialize
  dotMatrix.setIntensity(3); // dot matix intensity 0-15
  // bootup
  turnOnAllLeds();
  delay(1000);
  dotMatrix.writeSprite(0, 0, xicht_happy);
  tone(SPEAKER, 880, 100);
  turnOffAllLeds();

  // Button game
	for (int i = 0; i < 4; i++){
		myButtons[i].setPressTicks(60);
	}

	myButtons[0].attachPress(butt0Click);
	myButtons[1].attachPress(butt1Click);
	myButtons[2].attachPress(butt2Click);
	myButtons[3].attachPress(butt3Click);

	activeLed = turnOnRandomLed();
}

void loop() {
  for (int i = 0; i < 4; i++){
		myButtons[i].tick();
		myLeds[i].tick();
	}
}
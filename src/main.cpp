#include <Arduino.h>
#include <config.h>
#include <flasher.h>
#include <OneButton.h>
#include <tools.h>

OneButton myButtons[] = {OneButton(BUTTON_R, true), OneButton(BUTTON_G, true), OneButton(BUTTON_B, true), OneButton(BUTTON_Y, true)};
Flasher myLeds[] = {Flasher(LIGHT_R, 300, 300), Flasher(LIGHT_G, 300, 300), Flasher(LIGHT_B, 300, 300), Flasher(LIGHT_Y, 300, 300)};

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
  // bootup
  tone(SPEAKER, 880, 100);
  turnOnAllLeds();
  delay(500);
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
#include "ToneSfx.h"

ToneSfx::ToneSfx(int _pin, const char **_inputArray, int _inputArrayLength) {
    inputArray = _inputArray;
    pin = _pin;
    //inputArrayLength = sizeof(_inputArray) / sizeof(_inputArray[0]);
    inputArrayLength = _inputArrayLength;
}

bool ToneSfx::isPlaying() {
    return playing;
}

void ToneSfx::setInfinite(bool value = true) {
    infinitePlayback = value;
}

void ToneSfx::setSeedPin(int pin) {
    seedPin = pin;
  }

void ToneSfx::start() {
    playing = true;
    // TODO: here maybe reset the playback with each start?
}

void ToneSfx::stop() {
    noTone(pin);
    playing = false;
}

void ToneSfx::tick() {
    if (playing) {
        // reads value from array once
        if (!readCommand) {
            readValue = inputArray[arrayIndex];
            readCommand = true;
            Serial.print("^ READING COMMAND: ");
            Serial.println(readValue);
        }

        // Prepares values for effects
        // ***** TONE *****
        if (*readValue == 'T') {
            readValue++; readValue++; // skip T:
            // read frequency
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            frequency = num;
            readValue++; // skip semicolon

            // read duration
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            duration = num;

            commandType = 0;
        }
        // ***** PAUSE *****
        else if (*readValue == 'P') {
            readValue++; readValue++; // skip P:
            // read pause duration
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            duration = num;
            commandType = 1;
        }
        // ***** SWEEP *****
        else if (*readValue == 'S') {
            readValue++; readValue++; // skip S:
            // read freqStart
            num = 0;
                while(isdigit(*readValue)) {
            num = (num * 10) + (*readValue++ - '0');
            }
            freqStart = num;
            readValue++; // skip semicolon

            // read freqEnd
            num = 0;
                while(isdigit(*readValue)) {
            num = (num * 10) + (*readValue++ - '0');
            }
            freqEnd = num;
            readValue++; // skip semicolon

            // read step
            num = 0;
                while(isdigit(*readValue)) {
            num = (num * 10) + (*readValue++ - '0');
            }
            step = num;
            readValue++; // skip semicolon

            // read duration
            num = 0;
                while(isdigit(*readValue)) {
            num = (num * 10) + (*readValue++ - '0');
            }
            duration = num;

            frequency = freqStart;
            commandType = 2;
        }
        // ***** RANDOM or NOISE *****
        else if (*readValue == 'R' || *readValue == 'N' ) {
            bool isNoise = false;
            if (*readValue == 'N') {
                isNoise = true;
            }

            readValue++; readValue++; // skip R:
            // read freqStart
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            freqStart = num;
            readValue++; // skip semicolon

            // read freqEnd
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            freqEnd = num;
            readValue++; // skip semicolon

            // read minDuration
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            minDuration = num;
            readValue++; // skip semicolon

            // read maxDuration
            num = 0;
            while(isdigit(*readValue)) {
                num = (num * 10) + (*readValue++ - '0');
            }
            maxDuration = num;

            if(!isNoise) { // RANDOM
                frequency = randomGenerator(freqStart, freqEnd);
                duration = randomGenerator(minDuration, maxDuration);
                commandType = 0;
            }
            else { // NOISE
                readValue++; // skip semicolon

                // read maxDuration
                num = 0;
                while(isdigit(*readValue)) {
                    num = (num * 10) + (*readValue++ - '0');
                }
                noiseDuration = num;

                frequency = randomGenerator(freqStart, freqEnd);
                duration = randomGenerator(minDuration, maxDuration);
                commandType = 3;
            }

        }

        // tone generation
        switch (commandType) {
        case 0: // ***** TONE, RANDOM TONE *****
            tone(pin, frequency, duration);
            soundDelay.start(duration);
            if(soundDelay.elapsed()) {
                readCommand = false;
                commandType = -1;
            }
            break;

        case 1: // ***** PAUSE *****
            noTone(pin);
            soundDelay.start(duration);
            if(soundDelay.elapsed()) {
                readCommand = false;
                commandType = -1;
            }
            break;

        case 2: // ***** SWEEP *****
            tone(pin, frequency, duration);
            soundDelay.start(duration);
            if(soundDelay.elapsed()) {

                if (freqStart < freqEnd) { // lower to higher sweep
                    frequency += step;
                } else { // higher to lower sweep
                    frequency -= step;
                }

                if (frequency == freqEnd) {
                    readCommand = false;
                    commandType = -1;
                }
            }
            break;

        case 3: // ***** NOISE *****
            tone(pin, frequency, duration);
            soundDelay.start(duration);
            soundDelay2.start(noiseDuration);
            if(soundDelay.elapsed()) {

                frequency = randomGenerator(freqStart, freqEnd);
                duration = randomGenerator(minDuration, maxDuration);

                if (soundDelay2.elapsed()) {
                    readCommand = false;
                    commandType = -1;
                }
            }
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

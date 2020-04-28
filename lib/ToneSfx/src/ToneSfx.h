/*
ToneSfx library
===============
version 1.0 (Apr 2020)
copyright (c) 2020 MileStorm
https://github.com/milestorm

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version. This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at
http://www.gnu.org/licenses .

This libraryvuses VirtualDelay library
from http://www.avdweb.nl/arduino/libraries/virtualdelay.html
*/

#ifndef ToneSfx_h
#define ToneSfx_h

#define DEBUG true

#include <Arduino.h>
#include "../lib/VirtualDelay/avdweb_VirtualDelay.h"

class ToneSfx {
    public:
        ToneSfx(int _pin, const char **_inputArray, int _inputArrayLength);
        ToneSfx() {};
        bool isPlaying();
        void setInfinite(bool value = true);
        void setSeedPin(int pin);
        void start();
        void stop();
        void tick();

    private:
        int pin;
        int seedPin = A0;

        const char **inputArray;
        int inputArrayLength;
        int arrayIndex = 0;

        bool playing = false;
        bool infinitePlayback = false;

        bool readCommand = false;
        int commandType = -1;
        const char *readValue;

        int num;
        int frequency;
        int duration;
        int freqStart;
        int freqEnd;
        int step;
        int minDuration;
        int maxDuration;
        int noiseDuration;

        VirtualDelay soundDelay, soundDelay2;

        uint32_t get_seed(int pin);
        int randomGenerator(int min, int max);
};



#endif

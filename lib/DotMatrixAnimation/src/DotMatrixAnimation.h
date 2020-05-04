/*
DotMatrixAnimation
==================
version 1.0 (Apr 2020)
copyright (c) 2020 MileStorm
https://github.com/milestorm

Library for non blocking animation on 8x8 matrix.

Inspired from editor https://xantorohara.github.io/led-matrix-editor/

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version. This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at
http://www.gnu.org/licenses .

This library uses VirtualDelay library
from http://www.avdweb.nl/arduino/libraries/virtualdelay.html
and MaxMatrix library from Oscar Kin-Chung Au
*/

#ifndef DotMatrixAnimation_h
#define DotMatrixAnimation_h

#include <Arduino.h>
#include <../lib/MaxMatrix/MaxMatrix.h>
#include <../lib/VirtualDelay/avdweb_VirtualDelay.h>

extern "C" {
    typedef void (*callbackFunction)(void);
}

class DotMatrixAnimation {
    private:
        bool isAnimating = false;
        bool isStillFrame = false;

        const uint64_t *animArray;
        int animLength;
        int cyclesCount = 1;
        int frameDelay = 100;

        bool isInfinite = false;

        // counter helpers
        int cyclesIndex = 0;
        int animIndex = 0;

        callbackFunction _callbackFunc = NULL;

        VirtualDelay animDelay;
        MaxMatrix dotMatrix;

        void displayImage(uint64_t image);

    public:
        DotMatrixAnimation(MaxMatrix &dotMatrix);
        DotMatrixAnimation() {};
        bool isRunning();
        void play(const uint64_t *animArray, int animLength, int cyclesCount = 1, bool isInfinite = false, int frameDelay = 100, callbackFunction newFunction = NULL);
        void stillFrame(const uint64_t *animArray, int frameDelay, int frameIndex = 0, callbackFunction newFunction = NULL);
        void stop();
        void tick();
};

#endif

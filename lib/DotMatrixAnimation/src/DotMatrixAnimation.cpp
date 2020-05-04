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

#include "DotMatrixAnimation.h"

DotMatrixAnimation::DotMatrixAnimation(MaxMatrix &dotMatrix) {
    this->dotMatrix = dotMatrix;
}

void DotMatrixAnimation::displayImage(uint64_t image) {
    for (int i = 0; i < 8; i++) {
        byte row = (image >> i * 8) & 0xFF;
        for (int j = 0; j < 8; j++) {
            this->dotMatrix.setDot(j, i, bitRead(row, j));
        }
    }
}

bool DotMatrixAnimation::isRunning() {
    return this->isAnimating;
}

void DotMatrixAnimation::play(const uint64_t *animArray, int animLength, int cyclesCount, bool isInfinite, int frameDelay, callbackFunction newFunction) {
    this->cyclesCount = cyclesCount;
    this->animArray = animArray;
    this->animLength = animLength;
    this->isInfinite = isInfinite;
    this->frameDelay = frameDelay;
    this->cyclesIndex = 0;
    this->animIndex = 0;
    this->isAnimating = true;
    this->isStillFrame = false;

    this->_callbackFunc = newFunction;
}

void DotMatrixAnimation::stop() {
    this->isAnimating = false;
    this->dotMatrix.clear();
}

void DotMatrixAnimation::stillFrame(const uint64_t *animArray, int frameDelay, int frameIndex, callbackFunction newFunction) {
    this->animArray = animArray;
    this->animIndex = frameIndex;
    this->frameDelay = frameDelay;
    this->isAnimating = true;
    this->isStillFrame = true;

    this->_callbackFunc = newFunction;

    uint64_t image;

    memcpy_P(&image, &this->animArray[this->animIndex], 8);
    displayImage(image);
}

void DotMatrixAnimation::tick() {
    if (this->isAnimating == true) {
        this->animDelay.start(this->frameDelay);
        if (this->animDelay.elapsed()) {

            uint64_t image;

            memcpy_P(&image, &this->animArray[this->animIndex], 8);
            displayImage(image);

            if (this->isStillFrame) {
                this->isAnimating = false;
                this->dotMatrix.clear();

                if (this->_callbackFunc) this->_callbackFunc();
            }
            else {
                if (this->animIndex < this->animLength - 1) {
                this->animIndex++;
                } else {
                    if (this->isInfinite) {
                        this->animIndex = 0;
                    } else {
                        if (this->cyclesIndex == this->cyclesCount - 1) {
                            this->isAnimating = false;
                            this->dotMatrix.clear();

                            if (this->_callbackFunc) this->_callbackFunc();
                        }
                        this->animIndex = 0;
                        this->cyclesIndex++;
                    }
                }
            }

        }
    }
}
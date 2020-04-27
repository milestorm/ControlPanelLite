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

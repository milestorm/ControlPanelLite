# DotMatrixAnimation library for Arduino

Non-blocking animations for 8x8 led dot matrix display

Inspired from editor [https://xantorohara.github.io/led-matrix-editor/](https://xantorohara.github.io/led-matrix-editor/). Use it to create animation code for this library.

This library uses [VirtualDelay library](http://www.avdweb.nl/arduino/libraries/virtualdelay.html) and [MaxMatrix library](https://github.com/riyas-org/max7219/tree/master/MaxMatrix)

## Library Usage
### Instantiation/Creation
```
DotMatrixAnimation matrixAnimation(dot_matrix);
```

Where `dot_matrix` is MaxMatrix instance

### Methods
* `isRunning()` - Returns true if animation is playing, false if not.
* `play(animArray, animLength, [cyclesCount], [isInfinite], [frameDelay], [callbackFn])` - plays animation
    * `animArray` - Animation sequence generated from the editor. See upwards. __This value MUST be stored in PROGMEM__
    * `animLength` - Length of the sequence. Also generated from the editor.
    * `cyclesCount` - _OPTIONAL_ How many times will animation repeat. __Default is 1__
    * `isInfinite` - _OPTIONAL_ If set to true, animation will be infinite. Can be stopped with `stop()`. __Default is FALSE__
    * `frameDelay` - _OPTIONAL_ Sets the delay in ms between frames. Lower value is faster animation. __Default is 100__
    * `callbackFn` - _OPTIONAL_ Callback function, which is fired after the animation ends.
* `stillFrame(animArray, frameDelay, [frameIndex], [callbackFn])` - Displays still frame (image) from animation sequence for the specified duration.
    * `animArray` - Animation sequence generated from the editor. See upwards. __This value MUST be stored in PROGMEM__
    * `frameDelay` - Duration of the displayed frame (image)
    * `callbackFn` - _OPTIONAL_ Callback function, which is fired after the `frameDelay` timer ends.
* `stop()` - Stops animation
* `tick()` - Watcher. Must be placed in `loop()` function.

### Example
This example shows simple animation, that runs forever.

```
#include <Arduino.h>
#include <MaxMatrix.h>
#include <DotMatrixAnimation.h>

#define DOTMATRIX_DIN  16
#define DOTMATRIX_CS   14
#define DOTMATRIX_CLK  15
#define DOTMATRIX_DISPLAY_COUNT 1

MaxMatrix dot_matrix(DOTMATRIX_DIN, DOTMATRIX_CS, DOTMATRIX_CLK, DOTMATRIX_DISPLAY_COUNT);
DotMatrixAnimation matrixAnimation(dot_matrix);

const uint64_t ANIM_effect[] PROGMEM = {
  0x8142241818244281,
  0x4021121c38488402,
  0x2010111e78880804,
  0x1010101ff8080808,
  0x080808f81f101010,
  0x040888781e111020,
  0x028448381c122140
};
int ANIM_effect_len = sizeof(ANIM_effect)/8;

void setup() {
    dot_matrix.init();
    dot_matrix.setIntensity(3);

    matrixAnimation.play(ANIM_effect, ANIM_effect_len, 1, true);
}

void loop(){
    matrixAnimation.tick();
}
```
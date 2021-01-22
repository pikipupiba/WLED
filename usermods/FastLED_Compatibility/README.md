# FastLED Compatibility UserFX Wrapper

The FastLED Compatibility UserFX Wrapper (FL_Wrapper) allows for easier porting of FastLED effects to WLED.

## Functionality

FL_Wrapper adds an additional buffer for storing CRGB values, which saves the full CRGB data so it can be retrieved on the next pass through the effect, unlike WLED's `getPixelColor()` which is lossy.  It also defines some useful variables so FastLED sketches can be copied over with minimal modifications.

### Basic Functionality

FastLED sketches usually store pixel data in `CRGB leds[]`, and define `NUM_LEDS` at the top of the sketch.  `leds[]` is also persistent between passes through `loop()`, so effects can be made by the data between previous frames.  This won't work with WLED, as `NUM_LEDS` isn't a constant, it's determined by the settings of the current segment.  Likewise, a fixed `leds[]` array wouldn't work as there are multiple segments that would need to access it.  Also, WLED uses `setPixelColor()` to store RGB data instead of directly accessing an array, and that data can't be directly retrieved like you would from an array, as `getPixelColor()` returns lossy data.

To make it easier to port FastLED sketches, FL_Wrapper manages an array that can be used for persistent storage of CRGB data, that can be accessed using `leds[]` in your effect, and sets a local constant `NUM_LEDS`.  Use `BEGIN_FASTLED_COMPATIBILITY()` at the top of the effect to set this up.  At the end of the effect, use `END_FASTLED_COMPATIBILITY()`, which takes care of writing the `leds[]` array to WLED's buffers using `setPixelColor()`.

### 2D Functionality ("Virtual Screen")

Some FastLED sketches are made for rectangular matrices, where the width/height is set to `kMatrixWidth`/`kMatrixHeight`, `NUM_LEDS` is `kMatrixWidth*kMatrixHeight`, and an `XY()` function is defined that can be used in the sketch to set the LED matching an X/Y coordinate in the matrix: `leds[XY(x,y)]`.

The `BEGIN_FASTLED_XY_COMPATIBILITY()` macro sets up all the FastLED compatibility as `BEGIN_FASTLED_COMPATIBILITY()` does, and also creates these variables:

```
kMatrixWidth = _segmentmaps[_segment_index].matrixWidth;
kMatrixHeight = _segmentmaps[_segment_index].matrixHeight;
int NUM_LEDS (kMatrixWidth*kMatrixHeight);
```

The `XY()` function is created and depends on the above `kMatrixWidth` and `kMatrixHeight` values.  It is accessible to FastLED's `blur2d()` function.

The `END_FASTLED_XY_COMPATIBILITY()` macro writes the contents of the `leds[]` array to WLED's pixel buffer

## Usage

Define `USERFX1_H` with the pathname of `FL_Wrapper_UserFX1.h`, and `USERFX1_CPP_H` with the pathname of `FL_Wrapper_UserFX1.cpp.h`.  Add to myconfig.h:

```
#define USERFX1_H       "../usermods/FastLED_Compatibility/FL_Wrapper_UserFX1.h"
#define USERFX1_CPP_H   "../usermods/FastLED_Compatibility/FL_Wrapper_UserFX1.cpp.h"
```
or Add to PlatformIO Config:

```
'-D USERFX1_H="../usermods/FastLED_Compatibility/FL_Wrapper_UserFX1.h"'
'-D USERFX1_CPP_H="../usermods/FastLED_Compatibility/FL_Wrapper_UserFX1.cpp.h"'
```

Assigning the FL_Wrapper UserFX set to UserFX1 puts the FL_Wrapper code at the end of FX.cpp, before any additional UserFX sets.  Now effects depending on FL_Wrapper can be included in additional UserFX sets.

There are some example effects in `FL_Effects_UserFX2.*`, you can include them just as you did FL_Wrapper.  Add to myconfig.h:

```
#define USERFX2_H       "../usermods/FastLED_Compatibility/FL_Effects_UserFX2.h"
#define USERFX2_CPP_H   "../usermods/FastLED_Compatibility/FL_Effects_UserFX2.cpp.h"
```
or Add to PlatformIO Config:

```
'-D USERFX2_H="../usermods/FastLED_Compatibility/FL_Effects_UserFX2.h"'
'-D USERFX2_CPP_H="../usermods/FastLED_Compatibility/FL_Effects_UserFX2.cpp.h"'
```

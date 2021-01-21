#pragma once

// The contents of this file can be directly included into FX.cpp by defining USERFX1_CPP_H

#include <stdint.h>

// _segmentmaps is temporary, and in the future should be pulling 2D values from _segments[], once 2D functionality is integrated into WLED
segmentmap _segmentmaps[MAX_NUM_SEGMENTS] = {
  // matrixWidth, matrixHeight, ledsOffset (set to the sum of previous segments' matrixWidth*matrixHeight values)
  { 16, 16, 0 },
  { 32, 32, 16*16 }
};

uint16_t kMatrixWidth;
uint16_t kMatrixHeight;

void setKMatrixWidth(uint16_t width) {
  kMatrixWidth = width;
}

void setKMatrixHeight(uint16_t height) {
  kMatrixHeight = height;
}

// this array may need to be larger than MAX_LEDS if XY mapping is used to drive less than (matrixWidth*matrixHeight) number of pixels in a segment
CRGB ledsCompat[MAX_LEDS];

// This function can be used in an effect, and can be used by FastLED's blur2d function
uint16_t XY( uint8_t x, uint8_t y) { return (y * kMatrixWidth) + x; }


// to fit within UserFX constraints, this WS2812FX method must return uint16_t (would be void otherwise)
uint16_t WS2812FX::writeLedsArrayToWled_XY(CRGB * leds) {
  for (int i=0; i<SEGLEN; i++) {
     setPixelColor(i, leds[i].red, leds[i].green, leds[i].blue);
  }

  return 0;
}

#define BEGIN_FASTLED_XY_COMPATIBILITY()                                      \
          CRGB * leds = &ledsCompat[_segmentmaps[_segment_index].ledsOffset]; \
          int NUM_LEDS (kMatrixWidth*kMatrixHeight);                          \
          setKMatrixWidth(_segmentmaps[_segment_index].matrixWidth);          \
          setKMatrixHeight(_segmentmaps[_segment_index].matrixHeight);

#define END_FASTLED_XY_COMPATIBILITY() \
          writeLedsArrayToWled_XY(leds);

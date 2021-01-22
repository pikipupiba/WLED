#pragma once

// The contents of this file can be directly included into FX.cpp by defining USERFX2_CPP_H

#include <stdint.h>

// SmartMatrixSwirl by Mark Kriegsman https://gist.github.com/kriegsman/5adca44e14ad025e6d3b
// https://forum.makerforums.info/t/2-dimensional-blur-this-video-shows-the-new-blur2d-capability-newly-available-on-the-fastled/64648
uint16_t WS2812FX::mode_2DSwirl(void) {
  BEGIN_FASTLED_XY_COMPATIBILITY(); // this macro sets up leds[], NUM_LEDS, kMatrixWidth, and kMatrixHeight to match the values configured for this segment

  const uint8_t kBorderWidth = 2;

  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  uint8_t blurAmount = beatsin8(2,10,255);
  blur2d( leds, kMatrixWidth, kMatrixHeight, blurAmount);

  // Use two out-of-sync sine waves
  uint8_t  i = beatsin8( 27, kBorderWidth, kMatrixHeight-kBorderWidth);
  uint8_t  j = beatsin8( 41, kBorderWidth, kMatrixWidth-kBorderWidth);
  // Also calculate some reflections
  uint8_t ni = (kMatrixWidth-1)-i;
  uint8_t nj = (kMatrixWidth-1)-j;
  
  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis();  
  leds[XY( i, j)] += CHSV( ms / 11, 200, 255);
  leds[XY( j, i)] += CHSV( ms / 13, 200, 255);
  leds[XY(ni,nj)] += CHSV( ms / 17, 200, 255);
  leds[XY(nj,ni)] += CHSV( ms / 29, 200, 255);
  leds[XY( i,nj)] += CHSV( ms / 37, 200, 255);
  leds[XY(ni, j)] += CHSV( ms / 41, 200, 255);

  END_FASTLED_XY_COMPATIBILITY(); // this macro writes the contents of leds[] to WLED

  return FRAMETIME;
}

uint16_t WS2812FX::mode_ScrollingWash(void) {
  BEGIN_FASTLED_COMPATIBILITY();

  const xyPair * xytable = _segmentmaps[_segment_index].xyTablePointer;

  for (int i=0; i<NUM_LEDS; i++) {
    uint16_t x = xytable[i].x;
    uint16_t y = xytable[i].y;
    leds[i] = CHSV( x + (millis() / 10), 255, 255);
  }

  END_FASTLED_COMPATIBILITY();

  return FRAMETIME;
}

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


// Fire2012 by Mark Kriegsman. Converted to WLED by Andrew Tuline. 
// original https://github.com/FastLED/FastLED/blob/master/examples/Fire2012/Fire2012.ino
uint16_t WS2812FX::mode_2Dfire2012(void) {
  FL_ALLOC_WITH_1_ARRAY_XY(
    unsigned long prevMillis;
    , byte, heat, NUM_LEDS
    );

  const uint8_t COOLING = 50;
  const uint8_t SPARKING = 50;

  CRGBPalette16 currentPalette  = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Orange, CRGB::Yellow);
  unsigned long curMillis = millis();

  if ((curMillis - FL_STATICVAR(prevMillis)) >= ((256-SEGMENT.speed) >>2)) {
    FL_STATICVAR(prevMillis) = curMillis;

    for (int mw = 0; mw < kMatrixWidth; mw++) {            // Move along the width of the flame

      // Step 1.  Cool down every cell a little
      for (int mh = 0; mh < kMatrixHeight; mh++) {
        heat[mw*kMatrixWidth+mh] = qsub8( heat[mw*kMatrixWidth+mh],  random16(0, ((COOLING * 10) / kMatrixHeight) + 2));
      }

      // Step 2.  Heat from each cell drifts 'up' and diffuses a little
      for (int mh = kMatrixHeight - 1; mh >= 2; mh--) {
        heat[mw*kMatrixWidth+mh] = (heat[mw*kMatrixWidth+mh - 1] + heat[mw*kMatrixWidth+mh - 2] + heat[mw*kMatrixWidth+mh - 2] ) / 3;
      }

      // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
      if (random8(0,255) < SPARKING ) {
        int mh = random8(3);
        heat[mw*kMatrixWidth+mh] = qadd8( heat[mw*kMatrixWidth+mh], random8(160,255) );
      }

      // Step 4.  Map from heat cells to LED colors
      for (int mh = 0; mh < kMatrixHeight; mh++) {
        byte colorindex = scale8( heat[mw*kMatrixWidth+mh], 240);
        leds[XY(mw,mh)] = ColorFromPalette(currentPalette, colorindex, 255);
      } // for mh
    } // for mw
  } // if millis
  // TODO: we might not need to update leds every time based on millis check?

  END_FASTLED_XY_COMPATIBILITY();

  return FRAMETIME;
}

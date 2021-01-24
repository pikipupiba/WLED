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

// https://github.com/marmilicious/FastLED_examples/blob/master/every_n_timer_variable.ino
uint16_t WS2812FX::mode_blinktriggers(void) {
  FL_ALLOC(
    boolean counterTriggered = 0;  // Event triggered? [1=true, 0=false]
    CEveryNMilliseconds triggerTimer;
    CEveryNMillis flMillisTrigger1;
    CEveryNSeconds timingObj;
  );

  int msUntilNextPass = FRAMETIME;

  const uint16_t timerA = 3000;  // How often to run Event A [milliseconds]
  const uint16_t timerB = 500;  // How long after A to run Event B [milliseconds]

  // This runs once when the effect starts, similar to setup() in a FastLED sketch
  FL_SETUP() {
    FL_STATICVAR(counterTriggered) = 0;

    // make a macro?  These calls replace static CEveryNMilliseconds triggerTimer(timerB);
    FL_STATICVAR(triggerTimer).reset();
    FL_STATICVAR(triggerTimer).setPeriod(timerB);
  }

  FL_EVERY_N_MILLIS(flMillisTrigger1, timerA) {
    // do Event A stuff
    fill_solid(leds, NUM_LEDS, CHSV(random8(),255,128));
    FL_STATICVAR(counterTriggered) = 1;  // Set to True
    FL_STATICVAR(triggerTimer).reset();  // Start trigger timer
  }

  if (FL_STATICVAR(counterTriggered) == 1) {  // Will only be True if Event A has started
    if (FL_STATICVAR(triggerTimer)) {  // Check if triggerTimer time reached
      // do Event B stuff
      for (uint8_t i=0; i<NUM_LEDS/2; i++){
        leds[random16(NUM_LEDS)] = CRGB::Red;
      }
      FL_STATICVAR(counterTriggered) = 0;  // Set back to False
    }
  }

#if 0
  // this block doesn't translate easily to WLED as it uses delays in a loop and nested FastLED.show() calls
  FL_EVERY_N_SECONDS(timingObj, 20) {
    // This initally defaults to 20 seconds, but then will change the run
    // period to a new random number of seconds from 10 and 30 seconds.
    // You can name "timingObj" whatever you want.
    FL_STATICVAR(timingObj).setPeriod( random8(10,31) );
    fill_solid(leds, NUM_LEDS, CRGB::Black);
    for (uint16_t i=0; i<NUM_LEDS*3; i++){
      leds[random8(NUM_LEDS)] = CRGB::Black;
      leds[random8(NUM_LEDS)] = CHSV(random8(), random8(140,255), random8(50,255));
      FastLED.show();
      delay(random8(20,80));
    }
    msUntilNextPass = 500
  }
#endif

  END_FASTLED_COMPATIBILITY();

  return msUntilNextPass;  
}

// https://gist.github.com/kriegsman/5408ecd397744ba0393e
uint16_t WS2812FX::mode_fl_twinkles(void) {
  FL_ALLOC_WITH_1_ARRAY_XY(

    , uint8_t, directionFlags, (NUM_LEDS+7) / 8);

  enum { GETTING_DARKER = 0, GETTING_BRIGHTER = 1 };

  const int STARTING_BRIGHTNESS = 64;
  const int FADE_IN_SPEED       = 32;
  const int FADE_OUT_SPEED      = 20;
  const int DENSITY            = 255;

  FL_FUNCTIONS_BEGIN // vvvvvvvvv Local Functions go here
    void brightenOrDarkenEachPixel( fract8 fadeUpAmount, fract8 fadeDownAmount)
    {
     for( uint16_t i = 0; i < NUM_LEDS; i++) {
        if( getPixelDirection(i) == GETTING_DARKER) {
          // This pixel is getting darker
          leds[i] = makeDarker( leds[i], fadeDownAmount);
        } else {
          // This pixel is getting brighter
          leds[i] = makeBrighter( leds[i], fadeUpAmount);
          // now check to see if we've maxxed out the brightness
          if( leds[i].r == 255 || leds[i].g == 255 || leds[i].b == 255) {
            // if so, turn around and start getting darker
            setPixelDirection(i, GETTING_DARKER);
          }
        }
      }
    }

    CRGB makeBrighter( const CRGB& color, fract8 howMuchBrighter) 
    {
      CRGB incrementalColor = color;
      incrementalColor.nscale8( howMuchBrighter);
      return color + incrementalColor;
    }

    CRGB makeDarker( const CRGB& color, fract8 howMuchDarker) 
    {
      CRGB newcolor = color;
      newcolor.nscale8( 255 - howMuchDarker);
      return newcolor;
    }

    bool getPixelDirection( uint16_t i) {
      uint16_t index = i / 8;
      uint8_t  bitNum = i & 0x07;
      // using Arduino 'bitRead' function; expanded code below
      return bitRead( FL_STATICVAR(directionFlags)[index], bitNum);
      // uint8_t  andMask = 1 << bitNum;
      // return (directionFlags[index] & andMask) != 0;
    }

    void setPixelDirection( uint16_t i, bool dir) {
      uint16_t index = i / 8;
      uint8_t  bitNum = i & 0x07;
      // using Arduino 'bitWrite' function; expanded code below
      bitWrite( FL_STATICVAR(directionFlags)[index], bitNum, dir);
      //  uint8_t  orMask = 1 << bitNum;
      //  uint8_t andMask = 255 - orMask;
      //  uint8_t value = directionFlags[index] & andMask;
      //  if( dir ) {
      //    value += orMask;
      //  }
      //  directionFlags[index] = value;
    }
  FL_FUNCTIONS_END // ^^^^^^^^^ Local Functions go here

  // Make each pixel brighter or darker, depending on
  // its 'direction' flag.
  FL_FUNCTION(brightenOrDarkenEachPixel)( FADE_IN_SPEED, FADE_OUT_SPEED);
  
  // Now consider adding a new random twinkle
  if( random8() < DENSITY ) {
    int pos = random16(NUM_LEDS);
    if( !leds[pos]) {
      leds[pos] = ColorFromPalette( currentPalette, random8(), STARTING_BRIGHTNESS, NOBLEND);
      FL_FUNCTION(setPixelDirection)(pos, GETTING_BRIGHTER);
    }
  }

  END_FASTLED_COMPATIBILITY();

  return FRAMETIME;
}

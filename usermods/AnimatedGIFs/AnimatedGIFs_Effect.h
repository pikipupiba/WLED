#pragma once

// This example effect's name is "AGIFS_TEST" - `#define USERFX1 AGIFS_TEST` will assign this AGIFS_TEST effect to UserFX slot number 1

// TODO: include file with function definition
//CRGB * agifsGetVirtualScreen(uint16_t width, uint16_t height);

uint16_t mode_agifs2D(void) {
  FL_GET_VIRTUAL_SCREEN(agifsGetVirtualScreen);      // This effect depends on leds[] being sized to kMatrixWidth * kMatrixHeight, and uses XY()

  FL_ALLOC(
    uint8_t intensityPrev;
    );

  if(FL_STATICVAR(intensityPrev) != SEGMENT.intensity) {
    FL_STATICVAR(intensityPrev) = SEGMENT.intensity;
    agifsSetNextGifIndex(SEGMENT.intensity, true);
  }

  END_FASTLED_COMPATIBILITY(); // this macro writes the contents of leds[] to WLED

  return FRAMETIME;
}

// set this to the name of your effect matching the name of the function defined above
#define AGIFS_TEST_MODE_NAME            mode_agifs2D

// set this to the name of your effect as you want it shown in the web interface
#define AGIFS_TEST_MODE_STRING          "*2D AnimatedGIFs"

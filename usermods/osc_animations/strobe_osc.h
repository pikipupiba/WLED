#pragma once

#include "../osc/osc_common.h"

// This file can be included into WLED by defining USERFX#_H (e.g. USERFX1_H) with the path+filename to this file in quotes
// Also define USERFX#_NAME (e.g. USERFX1_NAME) with the name of your effect used in the definitions below
// e.g. if the name of your effect is BLINK2, the definitions below should be named "BLINK2_MODE_STRING" and "BLINK2_MODE_NAME"
// ENABLE_USERFX must be defined to enable the UserFX functionality

// This example effect's name is "STROBE_OSC" - `#define USERFX1 STROBE_OSC` will assign this STROBE_OSC effect to UserFX slot number 1

uint16_t mode_strobe_osc(void) {
  
  float speed = osc_data.strobe_speed + 1;
  float duty = osc_data.strobe_duty;
  float fade = osc_data.strobe_fade;
  float offset = osc_data.strobe_offset;

  // SEGENV.aux0 is the last strobe time
  // SEGENV.aux1 is the something

  Serial.println("osc_data.strobe_speed");

  SEGENV.step++;
  fade_out(fade);

  if(SEGENV.step % int(speed) < duty)
  {
    for(int i = 0; i < SEGMENT.length(); i++)
    {
      setPixelColor( i, color_from_palette(i, true, (paletteBlend == 1 || paletteBlend == 3), 0));
    }
  }

  return FRAMETIME;
}

// set this to the name of your effect matching the name of the function defined above
#define STROBE_OSC_MODE_NAME            mode_strobe_osc

// set this to the name of your effect as you want it shown in the web interface
#define STROBE_OSC_MODE_STRING          "*Strobe OSC"

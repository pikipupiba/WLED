#pragma once

// This file can be included into WLED by defining USERFX#_H (e.g. USERFX1_H) with the path+filename to this file in quotes
// Also define USERFX#_NAME (e.g. USERFX1_NAME) with the name of your effect used in the definitions below
// e.g. if the name of your effect is RAINBOW_WAVE_OSC, the definitions below should be named "RAINBOW_WAVE_OSC_MODE_STRING" and "RAINBOW_WAVE_OSC_MODE_NAME"
// ENABLE_USERFX must be defined to enable the UserFX functionality

// This example effect's name is "RAINBOW_WAVE_OSC" - `#define USERFX1 RAINBOW_WAVE_OSC` will assign this RAINBOW_WAVE_OSC effect to UserFX slot number 1

uint16_t mode_rainbow_wave_osc(void) {
  return blink(SEGCOLOR(0), SEGCOLOR(1), false, true);
}

// set this to the name of your effect matching the name of the function defined above
#define RAINBOW_WAVE_OSC_MODE_NAME            mode_rainbow_wave_osc

// set this to the name of your effect as you want it shown in the web interface
#define RAINBOW_WAVE_OSC_MODE_STRING          "*Rainbow Wave OSC"

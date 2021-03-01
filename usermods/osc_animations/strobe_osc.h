#pragma once

// This file can be included into WLED by defining USERFX#_H (e.g. USERFX1_H) with the path+filename to this file in quotes
// Also define USERFX#_NAME (e.g. USERFX1_NAME) with the name of your effect used in the definitions below
// e.g. if the name of your effect is BLINK2, the definitions below should be named "BLINK2_MODE_STRING" and "BLINK2_MODE_NAME"
// ENABLE_USERFX must be defined to enable the UserFX functionality

// This example effect's name is "STROBE_OSC" - `#define USERFX1 STROBE_OSC` will assign this STROBE_OSC effect to UserFX slot number 1

uint16_t mode_strobe_osc(void) {
  return blink(SEGCOLOR(0), SEGCOLOR(1), false, true);
}

// set this to the name of your effect matching the name of the function defined above
#define STROBE_OSC_MODE_NAME            mode_strobe_osc

// set this to the name of your effect as you want it shown in the web interface
#define STROBE_OSC_MODE_STRING          "*Strobe OSC"

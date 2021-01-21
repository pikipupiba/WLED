#pragma once

// This file can be included into FX.h by defining USERFX1_H with the path+name to this file in quotes

// The FastLED Wrapper UserFX set doesn't add any effects, but does add a function to MODES_LIST to create a helper function inside WS2812FX

#define USERFX1_MODE_COUNT 0
#define USERFX1_ADD_MODES_TO_MAP()
#define USERFX1_JSON_MODE_NAMES

// on each line: prepend comma before mode, add mode function declaration, followed by a slash
#define USERFX1_MODES_LIST()                                  \
    ,writeLedsArrayToWled_XY(CRGB * leds)                     \

typedef struct Segmentmap {
  uint16_t matrixWidth;
  uint16_t matrixHeight;
  int ledsOffset;
} segmentmap;

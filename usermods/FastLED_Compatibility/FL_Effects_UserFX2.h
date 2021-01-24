#pragma once

// This file can be included into FX.h by defining USERFX2_H with the pathname to this file in quotes

// This is the count of how many modes are in the FX_user 
#define USERFX2_MODE_COUNT 5

// give each mode a unique number ascending from 0
#define USERFX2_MODE_2DSWIRL                    0
#define USERFX2_MODE_SCROLLING_WASH             1
#define USERFX2_MODE_2D_FIRE2012                2
#define USERFX2_MODE_BLINK_TRIGGERS             3
#define USERFX2_MODE_FL_TWINKLES                4

// map one mode per line, with a slash at the end of each line
#define USERFX2_ADD_MODES_TO_MAP()                                                                                              \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_2DSWIRL]                   = &WS2812FX::mode_2DSwirl;          \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_SCROLLING_WASH]            = &WS2812FX::mode_ScrollingWash;    \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_2D_FIRE2012]               = &WS2812FX::mode_2Dfire2012;       \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_BLINK_TRIGGERS]            = &WS2812FX::mode_blinktriggers;    \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_FL_TWINKLES]               = &WS2812FX::mode_fl_twinkles;      \

// on each line: prepend comma before mode, add mode function declaration, followed by a slash
#define USERFX2_MODES_LIST()                \
    ,mode_2DSwirl(void)                     \
    ,mode_ScrollingWash(void)               \
    ,mode_2Dfire2012(void)                  \
    ,mode_blinktriggers(void)               \
    ,mode_fl_twinkles(void)                 \

// prepend comma before each mode name, no comma after last name
#define USERFX2_JSON_MODE_NAMES R"=====(,"*2D Swirl","*2D Scrolling Wash","*2D Fire2012","*blink triggers","*FL Twinkles")====="

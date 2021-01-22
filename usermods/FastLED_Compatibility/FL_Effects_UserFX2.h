#pragma once

// This file can be included into FX.h by defining USERFX2_H with the pathname to this file in quotes

// This is the count of how many modes are in the FX_user 
#define USERFX2_MODE_COUNT 2

// give each mode a unique number ascending from 0
#define USERFX2_MODE_2DSWIRL                    0
#define USERFX2_MODE_SCROLLING_WASH             1

// map one mode per line, with a slash at the end of each line
#define USERFX2_ADD_MODES_TO_MAP()       \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_2DSWIRL]                   = &WS2812FX::mode_2DSwirl; \
    _mode[BUILTIN_MODE_COUNT + USERFX1_MODE_COUNT + USERFX2_MODE_SCROLLING_WASH]            = &WS2812FX::mode_ScrollingWash; \

// on each line: prepend comma before mode, add mode function declaration, followed by a slash
#define USERFX2_MODES_LIST()                \
    ,mode_2DSwirl(void)                     \
    ,mode_ScrollingWash(void)               \

// prepend comma before each mode name, no comma after last name
#define USERFX2_JSON_MODE_NAMES R"=====(,"*2D Swirl","*2D Scrolling Wash")====="

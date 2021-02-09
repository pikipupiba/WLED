#pragma once

typedef enum {
    playModeFirst = 0,
    playModeBoth = 1
} gifPlayModes;

void * agifsGetVirtualScreen(uint16_t width, uint16_t height);

void agifsSetNextGifIndex(int index, bool switchImmediately = false);

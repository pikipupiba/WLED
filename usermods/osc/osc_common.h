#pragma once

struct OSC_Data
{
    uint8_t hue;
    float hue_speed;
    float hue_offset;
    uint8_t brightness;
    uint16_t position;
    float position_offset;
    float duty_cycle;
    float bpm;
    float strobe_multiplier;
};

extern OSC_Data osc_data;
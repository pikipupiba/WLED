#pragma once

#include "Arduino.h"
#include <vector>

struct OSC_Data
{
  // std::vector<OSC_Member> data;
  float hue;
  float hue_speed;
  float hue_offset;
  float strobe_speed;
  float strobe_duty;
  float strobe_fade;
  float strobe_offset;

  
  float beat_offset = 0;

  void print(void)
  {
    Serial.printf("Hue: %f\n\r", hue);
    Serial.printf("Hue Speed: %f\n\r", hue_speed);
    Serial.printf("Hue Offset: %f\n\r", hue_offset);
    Serial.printf("Strobe Speed: %f\n\r", strobe_speed);
    Serial.printf("Strobe Duty: %f\n\r", strobe_duty);
    Serial.printf("Strobe Fade: %f\n\r", strobe_fade);
    Serial.printf("Strobe Offset: %f\n\r", strobe_offset);
  }
};

extern OSC_Data osc_data;
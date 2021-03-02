#pragma once

#include "Arduino.h"
#include <vector>

struct OSC_Member
{
    String name;
    float data;
    float value;
};

struct OSC_Data
{
    std::vector<OSC_Member> data;
    std::vector<OSC_Member> value; //assumed you meant this


};



extern OSC_Data osc_data;

// OSC_Member member = {"name", 0.0};

// osc_data.data.push_back(member);
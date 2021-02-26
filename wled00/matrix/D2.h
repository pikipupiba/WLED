#pragma once

#include "../FX.h"
class D2
{
public:

    float x;
    float y;

public:
    D2();
    D2(float, float);

    void log(void);
    void log(String);

    void operator += (const D2 &b)
    {
        x += b.x;
        y += b.y;
    }

    void operator + (const D2 &b)
    {
        x += b.x;
        y += b.y;
    }

    void operator * (const float &b)
    {
        x *= b;
        y *= b;
    }
};
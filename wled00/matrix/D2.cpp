#include "D2.h"

D2::D2(void)
    :x(0.0), y(0.0)
{}

D2::D2(float x, float y)
    :x(x), y(y)
{

}

void D2::log(void)
{
    Serial.print(" { ");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.println(" }");
}

void D2::log(String label)
{
    Serial.print(label);
    log();
}
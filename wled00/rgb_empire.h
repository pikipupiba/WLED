#pragma once

#include "FX.h"
#include <vector>

#include "matrix/Matrix.h"
#include "./matrix/characters.h"

/*
 * uptime animation.
 */
uint16_t WS2812FX::mode_uptime(void)
{

    return FRAMETIME;
}



uint16_t WS2812FX::mode_bouncing_border(void)
{

    return FRAMETIME;
}

uint16_t WS2812FX::mode_firework(void)
{


    return FRAMETIME;
}

// 1. Testing crossfade stuff with websocket api
// 2. Set transition time you want
// 3. Trigger new effect

// using wled user interface


















String WS2812FX::get_time(String format = "%X")
{ // We're making sine waves here. By Andrew Tuline.
 // TODO: https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    //strftime(buffer, sizeof(buffer), "%d-%m-%Y-%H-%M-%S", timeinfo);
    strftime(buffer, sizeof(buffer), "%X", timeinfo);
    String str(buffer);

    return str;
}

/*
 * Effects by Andrew Tuline
 */
void WS2812FX::print_string(String str, int x, int y)
{

    for (char const &c : str)
    {
        x += print_char(c, x, y);
    }
}

int WS2812FX::print_char(char c, int x, int y)
{
    image cur_image = images[int(c) - 32];

    print_image(cur_image, x, y);

    return cur_image.width;
}

void WS2812FX::print_image(image i, int xo, int yo)
{
    // for (int y = 0; y < i.height; y++)
    // {
    //     for (int x = 0; x < 5; x++)
    //     {
            
    //         our_matrix.pixels[y+yo][x+xo] = i.pixel_array[y * i.width + x];

    //         // if(x + xo == 0 && i.pixel_array[y * i.width + x] == 1){
    //         //     Serial.print(" (");
    //         //     Serial.print(x+xo);
    //         //     Serial.print(",");
    //         //     Serial.print(y+yo);
    //         //     Serial.print(") = ");
    //         //     Serial.print(i.pixel_array[y * i.width + x]);
    //         //     Serial.print(" = ");
    //         //     Serial.println(our_matrix.pixels[y+yo][x+xo]);
    //         //     log_matrix(our_matrix);
    //         // }
    //     }
    // }
}
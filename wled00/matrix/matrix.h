#pragma once

#include "FX.h"
#include <vector>
#include "D2.h"

class Matrix
{

public:
    int w; // width
    int h; // height
    String shape;
    CHSV pixels[32][32];

    int first_led;
    int length;

public:
    Matrix(int, int, String, int);
    ~Matrix();
    
    void draw_point(D2, CHSV color);
    void draw();
    void log();
    void clear();
    void fade(int);

    int print_string(); // returns the height of the string.
    int print_char();   // returns the width of the char.
    void print_image();

    void plot_points(std::vector<D2>, CHSV);
    void print_x_line(float, float, CHSV);
    void print_y_line(float, float, CHSV);
    void print_line(D2, D2, CHSV, bool);
    void print_line(D2, float, CHSV, bool);
    void spokes1(float, float, uint16_t, float, CHSV, float);
    void spokes2(float, float, uint16_t, float, CHSV, float);
    void spokes3(float, float, uint16_t, float, CHSV, float);
    void spokes4(float, float, uint16_t, float, CHSV, float);
};
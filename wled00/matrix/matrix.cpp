#include "Matrix.h"

Matrix::Matrix(int width, int height, String shape, int first_led)
    :w(width), 
    h(height), 
    shape(shape),
    first_led(first_led)
{
    length = w * h;

    pixels[32][32] = CHSV(0,255,0);
}

Matrix::~Matrix(){}

void Matrix::draw_point(D2 p, CHSV color)
{

    float partial_x = p.x - floor(p.x);
    float partial_y = p.y - floor(p.y);

    int p1x = ceil(p.x);
    int p1y = ceil(p.y);
    int p2x = floor(p.x);
    int p2y = ceil(p.y);
    int p3x = ceil(p.x);
    int p3y = floor(p.y);
    int p4x = floor(p.x);
    int p4y = floor(p.y);

    CHSV p1c = CHSV(color.h, color.s, 255 * partial_x * partial_y);
    CHSV p2c = CHSV(color.h, color.s, 255 * (1 - partial_x) * partial_y);
    CHSV p3c = CHSV(color.h, color.s, 255 * partial_x * (1 - partial_y));
    CHSV p4c = CHSV(color.h, color.s, 255 * (1 - partial_x ) * (1 - partial_y));

    if(p1y < h && p1x < w) pixels[p1y][p1x] = p1c;
    if(p2y < h && p2x < w) pixels[p2y][p2x] = p2c;
    if(p3y < h && p3x < w) pixels[p3y][p3x] = p3c;
    if(p4y < h && p4x < w) pixels[p4y][p4x] = p4c;
    
}

void Matrix::draw(){
    //log();

    // for (uint16_t i = 0; i < 1024; i++)
    // {
    //     setPixelColor(i, RED);
    // }

    // if (shape == "snake")
    // {
    //     for (int i = first_led; i < first_led + length; i++)
    //     {
    //         int y = int(i / w);
    //         int x = int(i % w);

    //         // Serial.print("i: ");
    //         // Serial.print(i);
    //         // Serial.print(" (");
    //         // Serial.print(x);
    //         // Serial.print(",");
    //         // Serial.print(y);
    //         // Serial.print(") = ");
    //         // Serial.println(our_matrix.pixels[y][x]);

    //         if(y%2 == 0){
    //             if(pixels[y][x] > 0) 
    //                 setPixelColor(i, SEGCOLOR(0));
    //         }
    //         else {
    //             if(pixels[y][31 - x] > 0) 
    //                 setPixelColor(i, SEGCOLOR(0));
    //         }
    //     }
    // }
}

void Matrix::log(){
    for (auto &y : pixels)
    {
        for (auto &x : y)
        {
            Serial.print(" ");
            x.v ? Serial.print(x.v) : Serial.print(".");
        }
        Serial.println("");
        Serial.println("");
    }

    Serial.println("---------------------------------------------------------------");
    Serial.println("---------------------------------------------------------------");
}

void Matrix::clear()
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            pixels[y][x] = CHSV(0,255,0);
        }
    }
}

void Matrix::fade(int amount)
{
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            if(pixels[y][x].v > 0)
                pixels[y][x] = CHSV(pixels[y][x].h-2, pixels[y][x].s-5, pixels[y][x].v-amount > 10 ? pixels[y][x].v-amount : 0);
        }
    }
}

float get_x_from_y(int y, float m, float b)
{
	return m*y+b;
}

float get_y_from_x(int x, float m, float b)
{
	return (x-b)/m;
}


void Matrix::print_x_line(float m, float b, CHSV color)
{
    std::vector<D2> line;

	if(m > 1 || m < -1){
    	for(int x = 0; x < 32; x++)
        {
        	line.push_back({x, get_y_from_x(x, m, b)});
        }
    }
    else
    {
    	for(int y = 0; y < 32; y++)
        {
        	line.push_back({get_x_from_y(y, m, b), y});
        }
    }
    
    plot_points(line, color);
}

void Matrix::print_y_line(float m, float b, CHSV color)
{
    std::vector<D2> line;

	if(m > 1 || m < -1){
    	for(int x = 0; x < 32; x++)
        {
        	line.push_back({x, get_x_from_y(x, m, b)});
        }
    }
    else
    {
    	for(int y = 0; y < 32; y++)
        {
        	line.push_back({get_y_from_x(y, m, b), y});
        }
    }
    
    plot_points(line, color);
}

void Matrix::plot_points(std::vector<D2> points, CHSV color)
{
    for(auto& point : points)
    {
        pixels[int(point.y)][int(point.x)] = color;
    }
}



void Matrix::print_line(D2 point, D2 slope, CHSV color, bool loop = false)
{
    float m;
    
    if(slope.x == 0)
    {
        m = 10000.0;
    }
    else
    {
        m = slope.y/slope.x;
    }

    print_line(point, m, color, loop);
}

float get_x(float y, D2 point, float m)
{
    return (y - point.y) / m + point.x;
}

float get_y(float x, D2 point, float m)
{
    return m * (x - point.x) + point.y;
}

void print_d2_vector(std::vector<D2> vector)
{
    for(auto& d2 : vector)
    {
        Serial.print(" { ");
        Serial.print(d2.x);
        Serial.print(", ");
        Serial.print(d2.y);
        Serial.print(" } ");
    }
    Serial.println();
}

void Matrix::print_line(D2 point, float m, CHSV color, bool loop = false)
{
    std::vector<D2> line;

    if(m > 1 || m < -1)
    {
        for(int y = 0; y < 32; y++)
        {
            float x = get_x(y, point, m);

            if(x >= 0.0 && x < 32.0) line.push_back({x, y});
        }
    }
    else
    {
        for(int x = 0; x < 32; x++)
        {
            float y = get_y(x, point, m);

            if(y >= 0.0 && y < 32.0) line.push_back({x, y});
        }
    }

    plot_points(line, color);
    //pixels[int(point.y)][int(point.x)] = CHSV(0,255,255);
}

void Matrix::spokes1(float num_spokes, float radius, uint16_t step, float rpm, CHSV color, float color_offset)
{

    float radius_factor = 65536 / radius;
    int slope_offset = 65536/(num_spokes);

    for(int i = 0; i < num_spokes; i++){
        float slope_x = cos16(step+slope_offset*(i))/rpm;
        float slope_y = sin16(step + slope_offset*(i))/rpm;

        float point_x = cos16(step+slope_offset*i)/radius_factor + 16;
        float point_y = sin16(step+slope_offset*i)/radius_factor + 16;

        CHSV cur_color = CHSV(color.h + color_offset * i, color.s, color.v);

        D2 slope = {slope_x, slope_y};
        D2 point = {point_x, point_y};

        print_line(point, slope, cur_color, false);
    }
}

void Matrix::spokes2(float num_spokes, float radius, uint16_t step, float rpm, CHSV color, float color_offset)
{

    float radius_factor = 65536 / radius;
    int slope_offset = 65536/(num_spokes);

    for(int i = 0; i < num_spokes; i++){
        float slope_x = cos16(step+slope_offset*(i+1))/rpm;
        float slope_y = sin16(step + slope_offset*(i+1))/rpm;

        float point_x = cos16(step+slope_offset*i)/radius_factor + 16;
        float point_y = sin16(step+slope_offset*i)/radius_factor + 16;

        CHSV cur_color = CHSV(color.h + color_offset * i, color.s, color.v);

        D2 slope = {slope_x, slope_y};
        D2 point = {point_x, point_y};

        print_line(point, slope, cur_color, false);
    }
}

void Matrix::spokes3(float num_spokes, float radius, uint16_t step, float rpm, CHSV color, float color_offset)
{

    float radius_factor = 65536 / radius;
    int slope_offset = 65536/(num_spokes);

    for(int i = 0; i < num_spokes; i++){
        float slope_y = cos16(step+slope_offset*(i))/rpm;
        float slope_x = sin16(step + slope_offset*(i))/rpm;

        float point_x = cos16(step+slope_offset*i)/radius_factor + 16;
        float point_y = sin16(step+slope_offset*i)/radius_factor + 16;

        CHSV cur_color = CHSV(color.h + color_offset * i, color.s, color.v);

        D2 slope = {slope_x, slope_y};
        D2 point = {point_x, point_y};

        print_line(point, slope, cur_color, false);
    }
}

void Matrix::spokes4(float num_spokes, float radius, uint16_t step, float rpm, CHSV color, float color_offset)
{

    float radius_factor = 65536 / radius;
    int slope_offset = 65536/(num_spokes);

    for(int i = 0; i < num_spokes; i++){
        float slope_y = cos16(step+slope_offset*(i+1))/rpm;
        float slope_x = sin16(step + slope_offset*(i+1))/rpm;

        float point_x = cos16(step+slope_offset*i)/radius_factor + 16;
        float point_y = sin16(step+slope_offset*i)/radius_factor + 16;

        CHSV cur_color = CHSV(color.h + color_offset * i, color.s, color.v);

        D2 slope = {slope_x, slope_y};
        D2 point = {point_x, point_y};

        print_line(point, slope, cur_color, false);
    }
}
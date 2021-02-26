#pragma once

#include "../FX.h"
#include "D2.h"
#include <vector>
#include "Matrix.h"

// Define what happens when a particle does when it hits the edge.
#define LOOP    0
#define BOUNCE  1
#define START   2   // not used yet
#define RANDOM  3   // randomly changes vel and accel

class Particle
{
protected:
    Matrix &m;

    // params[0] is position
    // params[1] is velocity
    // params[2] is acceleration, etc
    std::vector<D2> params;

    CHSV color;


public:
    Particle(Matrix &, std::vector<D2>, CHSV);
    Particle(Matrix &, D2, CHSV);

    void draw();
    void draw(std::vector<Particle> &);
    void set_color(CHSV);
    void run(int);
    void bounce(void);
};
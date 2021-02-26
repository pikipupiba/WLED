#include "Particle.h"

Particle::Particle(Matrix &input_matrix, std::vector<D2> input_params, CHSV input_color)
    : m(input_matrix),
      params(input_params),
      color(input_color){
          // DO SOMETHING
      };

Particle::Particle(Matrix &input_matrix, D2 input_position, CHSV input_color)
    : m(input_matrix),
      color(input_color)
{

    params.push_back(input_position);
};

void Particle::run(int timestep)
{
    // divide timestep by 1 second to get the timefactor
    float time_factor = float(timestep) / 1000.0;

    bounce();

    // update params
    for (int i = 0; i < params.size() - 1; i++)
    {

        // update the current param using the next param
        params[i].x += params[i + 1].x * time_factor;
        params[i].y += params[i + 1].y * time_factor;
    };
};

//add the latest position to the matrix
void Particle::draw()
{
    m.draw_point(params[0], color);
};

void Particle::draw(std::vector<Particle> &particles)
{
    for (auto &particle : particles)
    {
        m.draw_point(particle.params[0], particle.color);
    }
};

// update the particle main color
void Particle::set_color(CHSV new_color)
{
    color = new_color;
};

void Particle::bounce(void)
{
    if (params[0].x < 1 && params[1].x < 0)
    {
        params[1].x = -params[1].x;
        params[2].y = -params[2].y;
    }
    if (params[0].x > 30 && params[1].x > 0)
    {
        params[1].x = -params[1].x;
        params[2].y = -params[2].y;
    }
    if (params[0].y < 1 && params[1].y < 0)
    {
        params[1].y = -params[1].y;
        params[2].x = -params[2].x;
    }
    if (params[0].y > 30 && params[1].y > 0)
    {
        params[1].y = -params[1].y;
        params[2].x = -params[2].x;
    }
};
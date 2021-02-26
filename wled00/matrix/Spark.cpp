#include "Spark.h"

Spark::Spark(Matrix &new_matrix, D2 new_position, CHSV new_color, float velocity = 1, int num_balls = 8)
    :Particle(new_matrix, new_position, new_color)
{
    
}

std::vector<Particle> create_spark(float velocity, int num_balls)
{
    for (int i = 0; i < num_balls; i++)
        {
            // CHSV color = CHSV(uint8_t(i), 255, 255);

            // // All start in the middle
            // D2 position = {(float(m.w) - 1.0) / 2.0, (float(m.h) - 1.0) / 2.0};
            // D2 velocity = {float(cos16((65535.0 / num_balls) * i)) / 2500, float(sin16((65535.0 / num_balls) * i)) / 2500};
            // // D2 acceleration = { float(sin16((65535.0/32.0)*i))/5000 , float(cos16((65535.0/32.0)*i))/5000 };
            // D2 acceleration = {5, 5};

            // std::vector<D2> params;

            // params.push_back(position);
            // params.push_back(velocity);
            // params.push_back(acceleration);

            // particles.push_back(new Particle(m, params, color));
        }
}

void Spark::draw(void)
{
    Particle::draw(particles);
}
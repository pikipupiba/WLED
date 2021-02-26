#include <vector>
#include "Particle.h"


class Spark: public Particle
{
protected:
    std::vector<Particle> particles;

public:
    Spark(Matrix&, D2, CHSV, float, int);
    ~Spark();

    std::vector<Particle> create_spark();
    void draw(void);
};
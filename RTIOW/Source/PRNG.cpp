#include "PRNG.h"

unsigned int PRNG::s_state = 1;

void PRNG::UpdateSeed(unsigned int seed)
{
    s_state *= seed;
}

unsigned int PRNG::PCG()
{
    unsigned int state = s_state;
    s_state = s_state * 747796405u + 2891336453u;
    unsigned int word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float PRNG::Float()
{
    // Returns a random real in [0,1).
    return PCG() / static_cast<float>(std::numeric_limits<unsigned int>::max());
}

float PRNG::Float(float min, float max)
{
    // Returns a random real in [min,max).
    return min + (max - min) * Float();
}

glm::vec3 PRNG::Vec3(float min, float max)
{
    // Returns a random real in [min,max).
    return glm::vec3(Float(min, max), Float(min, max), Float(min, max));
}

glm::vec3 PRNG::UnitVec3()
{
    return glm::normalize(Vec3(-1, 1));
}

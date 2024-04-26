#include "PRNG.h"

unsigned int PRNG::s_state = 1;

void PRNG::SetSeed(unsigned int seed)
{
    s_state = seed;
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

glm::vec3 PRNG::InUnitSphere() {
    while (true) {
        glm::vec3 p = Vec3(-1.f, 1.f);
        if (glm::dot(p,p) < 1.f)
            return p;
    }
}

glm::vec3 PRNG::UnitVec3()
{
    return glm::normalize(InUnitSphere());
}

glm::vec3 PRNG::OnHemisphere(glm::vec3& normal)
{
    glm::vec3 OnUnitSphere = UnitVec3();
    if (dot(OnUnitSphere, normal) > 0.f)
        return OnUnitSphere;
    else
        return -OnUnitSphere;
}

#pragma once
#include "glm/glm.hpp"

class Ray {
public:
    Ray() {}
    Ray(const glm::vec3& origin, const glm::vec3& direction) : orig(origin), dir(direction) {}

    const glm::vec3& Origin() const { return orig; }
    const glm::vec3& Direction() const { return dir; }

    glm::vec3 At(float t) const { return orig + t * dir;}

private:
    glm::vec3 orig;
    glm::vec3 dir;
};

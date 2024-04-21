#pragma once
#include "glm/glm.hpp"

class Ray {
public:
    Ray() {}
    Ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin(origin), m_direction(direction) {}

    const glm::vec3& Origin() const { return m_origin; }
    const glm::vec3& Direction() const { return m_direction; }

    glm::vec3 At(float t) const { return m_origin + t * m_direction;}

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

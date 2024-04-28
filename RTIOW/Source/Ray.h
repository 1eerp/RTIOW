#pragma once
#include "glm/glm.hpp"

class Ray {
public:
    Ray() = default;
    Ray(const glm::vec3& origin, const glm::vec3& direction) : m_origin(origin), m_direction(direction) {}

    inline const glm::vec3& Origin() const { return m_origin; }
    inline const glm::vec3& Direction() const { return m_direction; }
    
    inline void SetOrigin(const glm::vec3& origin) { m_origin = origin; }
    inline void SetDirection(const glm::vec3& direction) { m_direction = direction;}

    glm::vec3 At(float t) const { return m_origin + t * m_direction;}

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

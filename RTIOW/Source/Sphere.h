#pragma once

#include "Hittable.h"
#include "glm/glm.hpp"

class Sphere : public Hittable {
public:
    Sphere(const glm::vec3& center, float radius) : m_center(center), m_radius(glm::max(0.f, radius)) {}
    bool Hit(const Ray& ray, HitRecord& record, Interval interval = Interval::Front()) const override;

private:
    glm::vec3 m_center;
    float m_radius;
};
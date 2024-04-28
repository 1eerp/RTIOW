#include "Sphere.h"

bool Sphere::Hit(const Ray& ray, HitRecord& record, Interval interval) const
{
    glm::vec3 oc = m_center - ray.Origin();
    float a = glm::dot(ray.Direction(), ray.Direction());
    float h = glm::dot(ray.Direction(), oc);
    float c = glm::dot(oc, oc) - m_radius * m_radius;
    float discriminant = h * h - a * c;

    if (discriminant < 0.f)
        return false;

    float sqrtd = glm::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrtd) / a;
    if (!interval.Surrounds(root)) {
        root = (h + sqrtd) / a;
        // [POTENTIAL Change/Optimization]: Determine if we are inside the geometry in this if statement. I would like to think that [(h - sqrtd) / a] is closer to the camera than [(h + sqrtd) / a], don't feel like proving it myself though
        // record.IsExteriorFace = false;
        if (!interval.Surrounds(root)) return false;
    }

    record.T = root;
    record.HitPosition = ray.At(record.T);
    // [POTENTIAL Change/Refactor]: Move logic for detecting whether we are inside or outside the geometry (still unsure on what to do, since the way this is detected might be different for different type of "Hittable"(s))
    record.Normal = (record.HitPosition - m_center) / m_radius;
    record.IsExteriorFace = glm::dot(record.Normal, ray.Direction()) < 0.f;
    record.Normal = record.IsExteriorFace ? record.Normal : -record.Normal;
    record.Material = m_material;

    return true;
}

#include "Material.h"

bool Lambertian::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 scatterDirection = glm::normalize(record.Normal + PRNG::UnitVec3());
    // If the normal and the random vec cancel each other, update the scatterDirection to be the original normal
    if (VecIsNearZero(scatterDirection))
        scatterDirection = record.Normal;

    scattered = Ray(record.HitPosition, scatterDirection);
    attenuation = m_albedo;
    return true;
}

bool Metal::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 reflectedDirection = Reflect(ray.Direction(), record.Normal);
    reflectedDirection = glm::normalize(reflectedDirection + (PRNG::UnitVec3() * m_roughness));
    scattered = Ray(record.HitPosition, reflectedDirection);
    attenuation = m_albedo;
    return glm::dot(scattered.Direction(), record.Normal) > 0.f;
}

bool Dielectric::Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const
{
    attenuation = glm::vec3(1.f);
    float ri = record.IsExteriorFace ? m_indexOfRefraction : (1.f / m_indexOfRefraction);

    glm::vec3 unitDirection = glm::normalize(ray.Direction());
    float cosTheta = glm::min(glm::dot(-unitDirection, record.Normal), 1.0f);
    float sinTheta = glm::sqrt(1.f - cosTheta * cosTheta);

    glm::vec3 dir;
    if (ri * sinTheta > 1.f || Reflectance(cosTheta,ri) > PRNG::Float())
        dir = Reflect(unitDirection, record.Normal);
    else
        dir = Refract(unitDirection, record.Normal, ri);
    scattered = Ray(record.HitPosition, dir);
    return true;
}
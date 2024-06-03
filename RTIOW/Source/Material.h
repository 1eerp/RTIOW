#pragma once
#include "glm/glm.hpp"
#include "Hittable.h"

////////////////////////
/// HELPER FUNCTIONS ///
////////////////////////

// Schlick's approximation for reflectance
inline float Reflectance(float cosine, float ior) {
    float r0 = (1.f - ior) / (1.f + ior);
    r0 = r0 * r0;
    return r0 + (1.f - r0) * static_cast<float>(glm::pow((1.f - cosine), 5));
}

inline glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n)
{
    // If the v is a unit vector the output should be a unit vector as well, but normalizing it anyway to account for floating point imprecision
    return glm::normalize(v - 2.f * dot(v, n) * n);
}

inline glm::vec3 Refract(const glm::vec3& v, const glm::vec3& n, float relativeIOR)
{
    float cosTheta = -glm::dot(v, n);
    glm::vec3 perp = relativeIOR * (v + cosTheta * n);
    glm::vec3 par = -glm::sqrt(std::fabs(1.f - glm::dot(perp, perp))) * n;
    return glm::normalize(perp + par);
}


/////////////////
/// MATERIALS ///
/////////////////
class Material
{
public:
    virtual ~Material() = default;

    virtual bool Scatter(const Ray& ray, const HitRecord& rec, glm::vec3& attenuation, Ray& scattered) const
    {
        return false;
    }
};

//////////////////
/// Lambertian ///
//////////////////

class Lambertian : public Material
{
public:
    Lambertian(const glm::vec3& albedo) : m_albedo(albedo) {}

    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const override; 

private:
    glm::vec3 m_albedo;
};

//////////////////
///   Metals   ///
//////////////////
class Metal : public Material
{
public:
    Metal(const glm::vec3& albedo, float roughness = 0.f) : m_albedo(albedo), m_roughness(roughness) {}

    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const override;

private:
    glm::vec3 m_albedo;
    float m_roughness;
};

///////////////////
/// Dielectrics ///
///////////////////
class Dielectric : public Material {
public:
    Dielectric(float ior) : m_indexOfRefraction(ior) {}

    bool Scatter(const Ray& ray, const HitRecord& record, glm::vec3& attenuation, Ray& scattered) const override;

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    float m_indexOfRefraction;
};
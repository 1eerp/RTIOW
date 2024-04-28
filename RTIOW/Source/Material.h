#pragma once
#include "glm/glm.hpp"
#include "Hittable.h"

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
///   Metal    ///
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
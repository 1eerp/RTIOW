#pragma once
#include "ImageWriter.h"
#include "Hittable.h"

class Camera {
public:
    Camera(const Ref<ImageWriter> writer, glm::vec3 position = { 0.f,0.f,0.f }, glm::vec3 direction = { 0.f, 0.f, 1.f }, float filmHeight = 2.f, float focalLength = 1.f);
    
    void Render(const Hittable& world);

private:
    glm::vec3 ColorRay(const Ray& ray, const Hittable& world);

private:
    Ref<ImageWriter> m_imageWriter;
    float m_aspectRatio;

    float m_filmHeight;
    float m_filmWidth;
    float m_focalLength;

    glm::vec3 m_position;
    glm::vec3 m_direction;
};
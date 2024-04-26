#pragma once
#include "ImageWriter.h"
#include "Hittable.h"

class Camera {
public:
    Camera(const Ref<ImageWriter> writer, unsigned short sampleCount = 1, unsigned short maxBounces = 1, glm::vec3 position = { 0.f,0.f,0.f }, glm::vec3 direction = { 0.f, 0.f, 1.f }, float filmHeight = 2.f, float focalLength = 1.f);
    
    void Render(const Hittable& world);

private:
    Ray CreateRay(unsigned short i, unsigned short j, glm::vec3& topLeftPixelCenter, glm::vec3& deltaX, glm::vec3 deltaY);
    glm::vec3 ColorRay(const Ray& ray, int depth, const Hittable& world);

private:
    Ref<ImageWriter> m_imageWriter;
    float m_aspectRatio;

    unsigned short m_sampleCount;
    unsigned short m_maxBounces;

    float m_filmHeight;
    float m_filmWidth;
    float m_focalLength;

    glm::vec3 m_position;
    glm::vec3 m_direction;
};
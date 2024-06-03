#pragma once
#include "ImageWriter.h"
#include "Hittable.h"

class Camera {
public:
    Camera(const Ref<ImageWriter> writer, unsigned short sampleCount = 1, unsigned short maxBounces = 1, float fov = 90.f, float focalDist = 1.f, float defocusAngle = 0.f, glm::vec3 position = { 0.f,0.f,0.f }, glm::vec3 lookAt = { 0.f, 0.f, 1.f }, glm::vec3 up = {0.f, 1.f, 0.f});
    
    void Render(const Hittable& world);

private:
    Ray CreateRay(unsigned short i, unsigned short j);
    glm::vec3 ColorRay(const Ray& ray, int depth, const Hittable& world);

private:
    // Film
    Ref<ImageWriter> m_imageWriter;

    // Sampling Quality
    unsigned short m_sampleCount;
    unsigned short m_maxBounces;

    // Camera Settings
    float m_fov;
    float m_aspectRatio;
    float m_focusDist;
    float m_defocusAngle;

    // Helper memory
    float m_focusPlaneHeight;
    float m_focusPlaneWidth;
    glm::vec3 m_defocusDiskU;
    glm::vec3 m_defocusDiskV;
    glm::vec3 m_topLeftPixelCenter;
    glm::vec3 m_deltaU;
    glm::vec3 m_deltaV;

    // Camera Position and Orientation
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_up;
    // [TODO] Add right vector and update functions for orientation that require roll
};
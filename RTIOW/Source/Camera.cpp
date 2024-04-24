#include <iostream>
#include "Camera.h"

Camera::Camera(const Ref<ImageWriter> writer, glm::vec3 position, glm::vec3 direction, float filmHeight, float focalLength)
	: m_imageWriter(writer), m_aspectRatio(static_cast<float>(writer->GetWidth()) / writer->GetHeight()), m_filmHeight(filmHeight), m_filmWidth(m_filmHeight * m_aspectRatio), m_focalLength(focalLength), m_position(position), m_direction(direction)
{}


glm::vec3 Camera::ColorRay(const Ray& ray, const Hittable& world)
{
	HitRecord hitRec;
	if (world.Hit(ray, hitRec))
		return 0.5f * (hitRec.Normal + glm::vec3(1.f, 1.f, 1.f));

	glm::vec3 unitDirection = glm::normalize(ray.Direction());
	float a = 0.5f * (unitDirection.y + 1.f);
	return (1.0f - a) * glm::vec3(1.f, 1.f, 1.f) + a * glm::vec3(0.5f, 0.7f, 1.f);
}

void Camera::Render(const Hittable& world)
{
	// Image Dimensions
	int imageWidth = m_imageWriter->GetWidth();
	int imageHeight = m_imageWriter->GetHeight();

	// Viewport right and down direction, w/ full length
	glm::vec3 viewportRight(m_filmWidth, 0, 0);
	glm::vec3 viewportDown(0, -m_filmHeight, 0);

	// Pixel length and width, directed from top left to bottom right
	glm::vec3 deltaX = viewportRight / static_cast<float>(imageWidth);
	glm::vec3 deltaY = viewportDown / static_cast<float>(imageHeight);

	// Position of the center of the Top Left Pixel
	glm::vec3 topLeftPixelCenter = m_position + (m_direction * m_focalLength) - (viewportRight / 2.f) - (viewportDown / 2.f) + (deltaX / 2.f) + (deltaY / 2.f);

	std::cout << std::fixed << std::cout.precision(3);

	// Render
	for (int j = 0; j < imageHeight; ++j) {
		std::cout << "PROGRESS: " << static_cast<float>(1 + j) / imageHeight * 100.f << "%\n";
		for (int i = 0; i < imageWidth; ++i) {
			// Calculate Color
			glm::vec3 currentPixel = topLeftPixelCenter + (static_cast<float>(j) * deltaY) + (static_cast<float>(i) * deltaX);
			Ray ray(m_position, glm::normalize(currentPixel - m_position));

			glm::vec3 unitRGB = ColorRay(ray, world);
			
			glm::uvec3 color = { static_cast<unsigned int>(255.999 * unitRGB.r), static_cast<unsigned int>(255.999 * unitRGB.g), static_cast<unsigned int>(255.999 * unitRGB.b) };

			// Write pixel color to file
			m_imageWriter->Push(color);
		}
	}
	std::cout << "DONE!\n";
}


#include <iostream>
#include "Camera.h"
#include "Material.h"

Camera::Camera(const Ref<ImageWriter> writer, unsigned short sampleCount, unsigned short maxBounces, float fov, float focalLength, glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
	: m_imageWriter(writer), m_sampleCount(sampleCount), m_maxBounces(maxBounces), m_fov(fov), m_aspectRatio(static_cast<float>(writer->GetWidth()) / writer->GetHeight()), m_focalLength(focalLength), m_position(position), m_direction(glm::normalize(lookAt - m_position)), m_up(glm::normalize(up))
{
	m_filmHeight = 2 * m_focalLength * static_cast<float>(glm::tan(glm::radians(m_fov / 2.f)));
	m_filmWidth = m_filmHeight * m_aspectRatio;
}

Ray Camera::CreateRay(unsigned short i, unsigned short j, glm::vec3& topLeftPixelCenter, glm::vec3& deltaX, glm::vec3 deltaY)
{
	// Calculate Randomized position inside of Pixel Square
	glm::vec3 currentPixelPosition = topLeftPixelCenter + ((static_cast<float>(j) + PRNG::Float() - 0.5f) * deltaY) + ((static_cast<float>(i) + PRNG::Float() - 0.5f) * deltaX);

	return Ray(m_position, glm::normalize(currentPixelPosition - m_position));
}

glm::vec3 Camera::ColorRay(const Ray& ray, int depth, const Hittable& world)
{
	if (depth < 0) return glm::vec3(0.f);

	HitRecord hitRec;
	if (world.Hit(ray, hitRec, Interval::Front()))
	{
		Ray scatteredRay;
		glm::vec3 attenuation;
		// The material scatters the ray
		if (hitRec.Material->Scatter(ray, hitRec, attenuation, scatteredRay))
			return attenuation * ColorRay(scatteredRay, depth - 1, world);

		// Otherwise it absorbs it
		return glm::vec3(0.f);
	}

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
	glm::vec3 right = glm::normalize(glm::cross(m_up, m_direction));
	m_up = glm::normalize(glm::cross(m_direction, right));

	glm::vec3 viewportRight = right * m_filmWidth;
	glm::vec3 viewportDown = -m_up * m_filmHeight;

	// Pixel length and width, directed from top left to bottom right
	glm::vec3 deltaX = viewportRight / static_cast<float>(imageWidth);
	glm::vec3 deltaY = viewportDown / static_cast<float>(imageHeight);

	// Position of the center of the Top Left Pixel
	glm::vec3 topLeftPixelCenter = m_position + (m_direction * m_focalLength) - (viewportRight / 2.f) - (viewportDown / 2.f) + (deltaX / 2.f) + (deltaY / 2.f);

	// Weight of each sample holds
	float pixelSampleScale = 1.f / m_sampleCount;

	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "Rendering...\n";

	// Render
	for (int j = 0; j < imageHeight; ++j) {
		// PROGRESS INDICATOR 
		std::cout << "\rPROGRESS: " << static_cast<float>(1 + j) / imageHeight * 100.f << "%";

		for (int i = 0; i < imageWidth; ++i) {
			// Initialize accumulator for current pixel
			glm::vec3 acUnitRGB(0.f);

			for (int sampleIndex = 0; sampleIndex < m_sampleCount; sampleIndex++)
			{
				// Create Ray
				Ray ray = CreateRay(i, j, topLeftPixelCenter, deltaX, deltaY);

				// Get Color of Ray
				acUnitRGB += ColorRay(ray, m_maxBounces, world);
			}
			// Normalize and clamp
			acUnitRGB = glm::clamp(pixelSampleScale * acUnitRGB, 0.f, 0.9999f);

			// Write pixel color to file
			m_imageWriter->Push(acUnitRGB);
		}
	}
	std::cout << "\nDONE!\n";
}


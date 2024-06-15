#include <iostream>
#include <thread>
#include <mutex>
#include "Camera.h"
#include "Material.h"
#include "ScopedTimer.h"

Camera::Camera(const Ref<ImageWriter> writer, unsigned short sampleCount, unsigned short maxBounces, float fov, float focalDist, float defocusAngle, glm::vec3 position, glm::vec3 lookAt, glm::vec3 up)
	: m_imageWriter(writer), m_sampleCount(sampleCount), m_maxBounces(maxBounces), m_sampleWeight(1.f / sampleCount), m_fov(fov), m_aspectRatio(static_cast<float>(writer->GetWidth()) / writer->GetHeight()), m_focusDist(focalDist), m_defocusAngle(defocusAngle), m_position(position), m_direction(glm::normalize(lookAt - m_position)), m_up(glm::normalize(up)), m_tileSizeX(32), m_tileSizeY(32), m_tileCountX(static_cast<unsigned int>(glm::ceil(static_cast<float>(m_imageWriter->GetWidth()) / m_tileSizeX))), m_tileCountY(static_cast<unsigned int>(glm::ceil(static_cast<float>(m_imageWriter->GetHeight()) / m_tileSizeY)))
{
	// Width and Height of the focus plane
	m_focusPlaneHeight = 2 * m_focusDist * static_cast<float>(glm::tan(glm::radians(m_fov / 2.f)));
	m_focusPlaneWidth = m_focusPlaneHeight * m_aspectRatio;

	// Viewport right and down direction, w/ full length
	glm::vec3 right = glm::normalize(glm::cross(m_up, m_direction));
	m_up = glm::normalize(glm::cross(m_direction, right));

	glm::vec3 viewportRight = right * m_focusPlaneWidth;
	glm::vec3 viewportDown = -m_up * m_focusPlaneHeight;

	// Pixel length and width vectors, directed from top left to bottom right
	m_deltaV = viewportRight / static_cast<float>(m_imageWriter->GetWidth());
	m_deltaU = viewportDown / static_cast<float>(m_imageWriter->GetHeight());

	// Position of the center of the Top Left Pixel
	m_topLeftPixelCenter = m_position + (m_direction * m_focusDist) - (viewportRight / 2.f) - (viewportDown / 2.f) + (m_deltaU / 2.f) + (m_deltaV / 2.f);

	// Calculate defocus disk right and up vectors
	float defocusRadius = m_focusDist * glm::tan(glm::radians(m_defocusAngle / 2.f));
	m_defocusDiskU = right * defocusRadius;
	m_defocusDiskV = m_up * defocusRadius;
}

Ray Camera::CreateRay(unsigned short i, unsigned short j)
{
	// Calculate Randomized position inside of Pixel Square
	glm::vec3 currentPixelPosition = m_topLeftPixelCenter + ((static_cast<float>(j) + PRNG::Float() - 0.5f) * m_deltaU) + ((static_cast<float>(i) + PRNG::Float() - 0.5f) * m_deltaV);
	glm::vec2 randomOnDisc = PRNG::InUnitCircle();
	glm::vec3 origin = (m_defocusAngle <= 0.f ? m_position : m_position + (randomOnDisc.x * m_defocusDiskU) + (randomOnDisc.y * m_defocusDiskV));
	return Ray(origin, glm::normalize(currentPixelPosition - origin));
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
	const int imageWidth = m_imageWriter->GetWidth();
	const int imageHeight = m_imageWriter->GetHeight();

	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "Rendering...\n";

	ScopedTimer timer("Normal Renderer & Writer");
	// Render
	for (int j = 0; j < imageHeight; ++j) {

		for (int i = 0; i < imageWidth; ++i) {
			// Initialize accumulator for current pixel
			glm::vec3 acUnitRGB(0.f);

			for (int sampleIndex = 0; sampleIndex < m_sampleCount; sampleIndex++)
			{
				// Create Ray
				Ray ray = CreateRay(i, j);

				// Get Color of Ray
				acUnitRGB += ColorRay(ray, m_maxBounces, world);
			}
			// Normalize and clamp
			acUnitRGB = glm::clamp(m_sampleWeight * acUnitRGB, 0.f, 0.9999f);

			// Write pixel color to file
			m_imageWriter->Push(acUnitRGB);
		}

		// PROGRESS INDICATOR 
		std::cout << "\rPROGRESS: " << static_cast<float>(1 + j) / imageHeight * 100.f << "%";
	}
	std::cout << "\nDONE!\n";
}

void Camera::RenderTiled(const Hittable& world)
{
	unsigned int x, y, startX, maxX, maxY, tileIndex, tileCount = m_tileCountX * m_tileCountY;

	// Atomically retreive and increment the tileIndex, so when multithreading each thread works on a new unique tileIndex
	while ((tileIndex = m_atomicTileIndex++) < tileCount)
	{
		// Get the starting x and y pixel for this tile
		y = (tileIndex / m_tileCountX) * m_tileSizeY;
		startX = (tileIndex % m_tileCountX) * m_tileSizeX;

		// If tile is in last row or column make sure the tile size is decreased if necessary
		maxY = glm::min(m_imageWriter->GetHeight(), y + m_tileSizeY);
		maxX = glm::min(m_imageWriter->GetWidth(), startX + m_tileSizeX);

		for (; y < maxY; ++y) {

			for (x = startX; x < maxX; ++x) {
				// Retreive the backing memory for the pixel, and accumulator for current pixel
				glm::vec3& acUnitRGB = m_imageWriter->GetDataElementRef(y, x);

				for (int sampleIndex = 0; sampleIndex < m_sampleCount; sampleIndex++)
				{
					// Create Ray
					Ray ray = CreateRay(x, y);

					// Compute color pixel color and accumulate in the corresponding backing memory
					acUnitRGB += ColorRay(ray, m_maxBounces, world);
				}
				// Normalize the accumulated color and clamp it
				acUnitRGB = glm::clamp(m_sampleWeight * acUnitRGB, 0.f, 0.9999f);
			}
		}

		{
			// Progress Indicator
			std::lock_guard op(m_progressMutex);
			std::cout << "\rTiles Rendered: " << ++m_tilesRendered << '/' << tileCount;
		}
	}
}

void Camera::RenderMultithreaded(const Hittable& world)
{
	std::cout << std::fixed;
	std::cout.precision(3);
	std::cout << "Rendering...\n";

	{
		ScopedTimer timer("MT Renderer");
		unsigned int workerCount = glm::min(4U, std::thread::hardware_concurrency() / 2U);
		std::vector<std::jthread> workers;
		workers.reserve(workerCount);
		for (unsigned int i = 0; i < workerCount; ++i)
			workers.push_back(std::jthread(&Camera::RenderTiled, this, std::ref(world)));
	}
	ScopedTimer timer("ImageWriter");
	m_imageWriter->SaveData();
}

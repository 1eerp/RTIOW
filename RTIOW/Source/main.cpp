#include <iostream>
#include <string>
#include <fstream>
#include "glm/glm.hpp"
#include "ImageWriter.h"
#include "Ray.h"

glm::vec3 ray_color(const Ray& r) {
	glm::vec3 unit_direction = glm::normalize(r.Direction());
	auto a = 0.5f * (unit_direction.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0, 1.0, 1.0) + a * glm::vec3(0.5, 0.7, 1.0);
}

int main()
{
	// Image Dimensions
	int imageWidth = 1280;
	int imageHeight = 720;
	float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

	// Viewport widths less than one are ok since they are real valued.
	float viewportHeight = 2.f;
	float viewportWidth = viewportHeight * aspectRatio;

	// Cam origin, direction and focal length
	glm::vec3 cameraOrigin(0.f, 0.f, 0.f);
	glm::vec3 cameraDirection(0.f, 0.0f, 1.f);
	float focalLength = 1.f;

	// Viewport right and down direction, w/ full length
	glm::vec3 viewportRight(viewportWidth, 0, 0);
	glm::vec3 viewportDown(0, -viewportHeight, 0);

	// Pixel length and width, directed from top left to bottom right
	glm::vec3 deltaX = viewportRight / static_cast<float>(imageWidth);
	glm::vec3 deltaY = viewportDown / static_cast<float>(imageHeight);

	glm::vec3 topLeftPixelCenter = cameraOrigin + (cameraDirection * focalLength) - (viewportRight / 2.f) - (viewportDown / 2.f) + (deltaX / 2.f) + (deltaY / 2.f);

	ImageWriter image("output/render.ppm", imageWidth, imageHeight);
	std::cout.precision(3);

	// Render
	for (int j = 0; j < imageHeight; ++j) {
		std::cout << "PROGRESS: " << static_cast<float>(1 + j) / imageHeight * 100.f << "%\n";
		for (int i = 0; i < imageWidth; ++i) {
			// Calculate Color
			glm::vec3 currentPixel = topLeftPixelCenter + (static_cast<float>(j) * deltaY) + (static_cast<float>(i) * deltaX);
			Ray ray(cameraOrigin, glm::normalize(currentPixel - cameraOrigin));

			glm::vec3 test = ray_color(ray);

			glm::uvec3 color = {static_cast<unsigned int>(255.999 * test.r), static_cast<unsigned int>(255.999 * test.g), static_cast<unsigned int>(255.999 * test.b)};

			// Write pixel color to file
			image.Push(color);
		}
	}
	std::cout << "DONE!\n";

}

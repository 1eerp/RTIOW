#include <chrono>
#include <iostream>
#include "RTIOW.h"


int main()
{
	// Update the PRNG with an initial seed
	unsigned int secondsSinceEpoch = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

	// Only needs to be set once
	PRNG::SetSeed(secondsSinceEpoch);

	// Image Dimensions
	unsigned short imageWidth = 1280;
	unsigned short imageHeight = 720;
	float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

	// Renderer/Camera Settings
	unsigned short maxRayBounce = 7;
	unsigned short sampleCount = 100;

	// WORLD/SCENE
	HittableList world;

	world.Add(MakeRef<Sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f));
	world.Add(MakeRef<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f));

	// IMAGE WRITER
	Ref<ImageWriter> writer = MakeRef<ImageWriter>("output/render.ppm", imageWidth, imageHeight);

	// CAMREA
	Camera camera(writer, sampleCount, maxRayBounce, { 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });
	
	// RENDER
	camera.Render(world);
}

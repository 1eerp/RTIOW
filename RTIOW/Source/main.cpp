#include <chrono>
#include <iostream>
#include "RTIOW.h"


int main()
{
	// Update the PRNG with an initial seed
	unsigned int secondsSinceEpoch = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

	// Only needs to be set once
	PRNG::SetSeed(secondsSinceEpoch);

	// WORLD/SCENE
	HittableList world;

	// Create Materials
	Ref<Lambertian> materialGround = MakeRef<Lambertian>(glm::vec3(0.8f, 0.8f, 0.0f));
	Ref<Lambertian> materialCenter = MakeRef<Lambertian>(glm::vec3(0.1f, 0.2f, 0.5f));
	Ref<Metal> materialLeft = MakeRef<Metal>(glm::vec3(0.8f), 0.f);
	Ref<Metal> materialRight = MakeRef<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.1f);

	// Make and add hittable objects
	world.Add(MakeRef<Sphere>(materialGround, glm::vec3(0.f, -100.5f, -1.f), 100.f));
	world.Add(MakeRef<Sphere>(materialCenter, glm::vec3(0.f, 0.f, -2.f), 0.5f));
	world.Add(MakeRef<Sphere>(materialLeft, glm::vec3(-1.f, 0.f, -1.f), 0.5f));
	world.Add(MakeRef<Sphere>(materialRight, glm::vec3(1.f, 0.f, -1.f), 0.5f));
	
	// Image Dimensions
	unsigned short imageWidth = 1280;
	unsigned short imageHeight = 720;
	float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

	// IMAGE WRITER
	Ref<ImageWriter> writer = MakeRef<ImageWriter>("output/render.ppm", imageWidth, imageHeight);

	// Renderer/Camera Settings
	unsigned short maxRayBounce = 10;
	unsigned short sampleCount = 100;

	// CAMREA
	Camera camera(writer, sampleCount, maxRayBounce, { 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });
	
	// RENDER
	camera.Render(world);
}

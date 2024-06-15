#include <chrono>
#include <iostream>
#include "RTIOW.h"
#include <thread>

int main()
{
	// Update the PRNG with an initial seed
	unsigned int secondsSinceEpoch = static_cast<unsigned int>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

	// Only needs to be set once
	PRNG::SetSeed(secondsSinceEpoch);

	// WORLD/SCENE
	HittableList world;

	// Create Materials
	Ref<Lambertian> groundMaterial = MakeRef<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
	Ref<Dielectric> material1 = MakeRef<Dielectric>(1.f / 1.5f);
	Ref<Lambertian> material2 = MakeRef<Lambertian>(glm::vec3(0.4f, 0.2f, 0.1f));
	Ref<Metal> material3 = MakeRef<Metal>(glm::vec3(0.7f, 0.6f, 0.5f), 0.f);

	world.Add(MakeRef<Sphere>(groundMaterial, glm::vec3(0.f, -1000.f, 0.f), 1000.f));
	world.Add(MakeRef<Sphere>(material1, glm::vec3(0.f, 1.f, 0.f), 1.f));
	world.Add(MakeRef<Sphere>(material2, glm::vec3(-4.f, 1.f, 0.f), 1.f));
	world.Add(MakeRef<Sphere>(material3, glm::vec3(4.f, 1.f, 0.f), 1.f));

	for (int a = -11; a < 11; a++)
	{
		for (int b = -11; b < 11; b++)
		{
			float choice = PRNG::Float();
			glm::vec3 center(a + 0.9f * float(), 0.2f, b + 0.9f * PRNG::Float());

			if (glm::length(center - glm::vec3(4, 0.2, 0.f)) > 0.9f)
			{
				Ref<Material> material;
				glm::vec3 albedo = PRNG::Vec3() * PRNG::Float(0.5f, 1.f);
				if (choice < 0.8f) {
					// Lambertian
					material = MakeRef<Lambertian>(albedo);
				}
				else if (choice < 0.95f)
				{
					// metal
					auto fuzz = PRNG::Float(0.f, 0.5f);
					material = MakeRef<Metal>(albedo, fuzz);
				}
				else {
					// glass
					material = MakeRef<Dielectric>(1.f / 1.5f);
				}
				world.Add(MakeRef<Sphere>(material, center, 0.2f));
			}
		}
	}

	// Image Dimensions
	unsigned short imageWidth = 1280;
	unsigned short imageHeight = 720;

	// IMAGE WRITER
	Ref<ImageWriter> writer = MakeRef<ImageWriter>("output/render.ppm", imageWidth, imageHeight);

	// Renderer/Camera Settings
	unsigned short maxRayBounce = 7;
	unsigned short sampleCount = 100;
	float fov = 20.f;
	float focusDist = 10.f;
	float defocusAngle = .6f;

	// CAMREA
	Camera camera(writer, sampleCount, maxRayBounce, fov, focusDist, defocusAngle, { 13.f, 2.f, -3.f }, { 0.f, 0.f, 0.f });
	
	// RENDER
	// ST
	// camera.Render(world);
	// MT
	camera.RenderMultithreaded(world);
}

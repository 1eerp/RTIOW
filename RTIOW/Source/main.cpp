#include <iostream>
#include <string>
#include <fstream>

#include "RTIOW.h"


int main()
{
	// Image Dimensions
	unsigned short imageWidth = 1280;
	unsigned short imageHeight = 720;
	float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

	// Renderer/Camera Settings
	unsigned short maxRayBounce = 1;
	unsigned short sampleCount = 100;

	// WORLD/SCENE
	HittableList world;

	world.Add(MakeRef<Sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f));
	world.Add(MakeRef<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f));

	// IMAGE WRITER
	Ref<ImageWriter> writer = MakeRef<ImageWriter>("output/render.ppm", imageWidth, imageHeight);

	// CAMREA
	Camera camera(writer, { 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f }, 100);
	
	// RENDER
	camera.Render(world);
}

#include <iostream>
#include <string>
#include <fstream>

#include "RTIOW.h"


int main()
{
	// Image Dimensions
	int imageWidth = 1280;
	int imageHeight = 720;
	float aspectRatio = static_cast<float>(imageWidth) / imageHeight;

	// WORLD/SCENE
	HittableList world;

	world.Add(MakeRef<Sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f));
	world.Add(MakeRef<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f));

	// IMAGE WRITER
	Ref<ImageWriter> writer = MakeRef<ImageWriter>("output/render.ppm", imageWidth, imageHeight);

	// CAMREA
	Camera camera(writer, { 0.f, 0.f, 0.f }, { 0.f, 0.f, -1.f });
	
	// RENDER
	camera.Render(world);
}

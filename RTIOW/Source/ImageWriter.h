#pragma once

#include <string>
#include <fstream>
#include "glm/glm.hpp"

class ImageWriter
{
public:
	ImageWriter(std::string path, unsigned int width, unsigned int height);
	~ImageWriter();

	void Push(const glm::uvec3& color);

public:
	std::string filePath;
	std::ofstream file;
};
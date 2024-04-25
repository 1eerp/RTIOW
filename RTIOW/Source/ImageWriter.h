#pragma once

#include <string>
#include <fstream>
#include "glm/glm.hpp"

class ImageWriter
{
public:
	ImageWriter(std::string path, unsigned short width, unsigned short height);
	~ImageWriter();

	void Push(const glm::uvec3& color);
	void Push(const glm::vec3& color);

	unsigned short GetHeight() const { return m_height; }
	unsigned short GetWidth() const { return m_width; }

private:
	std::string filePath;
	std::ofstream file;

	unsigned short m_width;
	unsigned short m_height;
};
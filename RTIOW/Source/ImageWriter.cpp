#include "ImageWriter.h"

ImageWriter::ImageWriter(std::string path, unsigned short width, unsigned short height)
	: filePath(path), file(path), m_width(width), m_height(height)
{
	// Write the file format header information
	file << "P3\n" << m_width << ' ' << m_height << "\n255\n";
}

ImageWriter::~ImageWriter()
{
	file.close();
}

void ImageWriter::Push(const glm::uvec3& color) 
{
	file << color.r << ' ' << color.g << ' ' << color.b << '\n';
}

void ImageWriter::Push(const glm::vec3& color)
{
	Push(glm::uvec3{ static_cast<unsigned int>(256 * color.r), static_cast<unsigned int>(256 * color.g), static_cast<unsigned int>(256 * color.b) });
}
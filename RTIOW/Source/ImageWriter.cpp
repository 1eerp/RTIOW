#include "ImageWriter.h"

ImageWriter::ImageWriter(std::string path, unsigned int width, unsigned int height)
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

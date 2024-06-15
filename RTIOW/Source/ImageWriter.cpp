#include "ImageWriter.h"

ImageWriter::ImageWriter(std::string path, unsigned int width, unsigned int height)
	: m_filePath(path), m_file(path), m_width(width), m_height(height), m_data(height, std::vector<glm::vec3>(width, glm::vec3(0.f)))
{
	// Write the file format header information
	m_file << "P3\n" << m_width << ' ' << m_height << "\n255\n";
}

ImageWriter::~ImageWriter()
{
	m_file.close();
}

void ImageWriter::Push(const glm::uvec3& color) 
{
	m_file << color.r << ' ' << color.g << ' ' << color.b << '\n';
}

void ImageWriter::Push(const glm::vec3& color)
{
	Push(glm::uvec3{ 
		static_cast<unsigned int>(256 * GammaCorrect(color.r)),
		static_cast<unsigned int>(256 * GammaCorrect(color.g)),
		static_cast<unsigned int>(256 * GammaCorrect(color.b))
		});
}

void ImageWriter::SaveData()
{
	for (unsigned int i = 0; i < m_height; ++i)
		for (unsigned int j = 0; j < m_width; ++j)
			Push(m_data[i][j]);
}

float ImageWriter::GammaCorrect(float component)
{
	constexpr float gammaEncodeFactor = 1.f / 2.2f;
	return powf(component, gammaEncodeFactor);
}
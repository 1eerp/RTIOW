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
	void Push(const glm::vec3& color);
	void SaveData();

	inline glm::vec3& GetDataElementRef(unsigned int row, unsigned int col) { return m_data[row][col]; }
	inline unsigned int GetHeight() const { return m_height; }
	inline unsigned int GetWidth() const { return m_width; }

private:
	float GammaCorrect(float color);

private:
	std::string m_filePath;
	std::ofstream m_file;

	unsigned int m_width;
	unsigned int m_height;

	std::vector<std::vector<glm::vec3>> m_data;
};
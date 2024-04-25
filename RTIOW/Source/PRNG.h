#pragma once
#include "glm/glm.hpp"

class PRNG
{
public:
	static void UpdateSeed(unsigned int seed);
	static unsigned int PCG();

	static float Float();
	static float Float(float min, float max);

	static glm::vec3 Vec3(float min, float max);
	static glm::vec3 UnitVec3();

private:
	static unsigned int s_state;
};
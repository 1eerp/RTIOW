#pragma once
#include "glm/glm.hpp"

class PRNG
{
public:
	static void SetSeed(unsigned int seed);
	static unsigned int PCG();

	static float Float();
	static float Float(float min, float max);

	static glm::vec3 Vec3(float min, float max);
	static glm::vec3 InUnitSphere();
	static glm::vec3 UnitVec3();
	static glm::vec3 OnHemisphere(glm::vec3& normal);

private:
	static unsigned int s_state;
};
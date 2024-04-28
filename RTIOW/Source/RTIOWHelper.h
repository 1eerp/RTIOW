#pragma once
#include <vector>
#include <memory>
#include <limits>
#include <cmath>

#include "glm/glm.hpp"

// RENAMED SMART POINTERS
template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> MakeScope(Args&& ... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
constexpr Ref<T> MakeRef(Args&& ... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

// CONSTANTS
const float PI = 3.1415926f;

// UTILITY FUNCTIONS
inline float clamp(float x, float min, float max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

inline bool VecIsNearZero(const glm::vec3& vec)
{
	constexpr float s = 1e-8f;
	return (std::fabs(vec[0]) < s) && (std::fabs(vec[1]) < s) && (std::fabs(vec[2]) < s);
}

inline glm::vec3 Reflect(const glm::vec3& v, const glm::vec3& n)
{
	return v - 2.f * dot(v, n) * n;
}

// UTILITY CLASSES
#include "PRNG.h"
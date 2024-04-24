#pragma once
#include <vector>
#include <memory>
#include <limits>

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
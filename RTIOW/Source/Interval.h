#pragma once
#include <limits>

struct Interval {
public:
    Interval(float min, float max = std::numeric_limits<float>::infinity()) : Min(min), Max(max) {}

    inline float Size() const { return Max - Min; }
    inline bool Contains(float x) const { return Min <= x && x <= Max; }
    inline bool Surrounds(float x) const { return Min < x && x < Max; }

    static const Interval Front() { return Interval(0.0001f, std::numeric_limits<float>::infinity()); }
public:
    float Min, Max;
};
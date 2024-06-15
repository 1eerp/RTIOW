#include "ScopedTimer.h"

ScopedTimer::~ScopedTimer()
{
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - m_start).count();
    std::cout << "ScopedTimer [" << m_name << "] elapsed time: " << duration / 1000.f << " s\n";
}

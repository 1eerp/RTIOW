#pragma once

#include <chrono>
#include <iostream>

class ScopedTimer {
public:
    // Constructor starts the timer
    ScopedTimer(const std::string& name)
        : m_name(name), m_start(std::chrono::high_resolution_clock::now()) {}

    // Destructor stops the timer and prints the elapsed time
    ~ScopedTimer();

private:
    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_start;
};
#pragma once

#include <iostream>
#include <chrono> // time measurement

#define frand() static_cast <float> (rand()) / static_cast <float> (RAND_MAX)
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::steady_clock::time_point timestamp;
#define getTimeDoubleMs(X) std::chrono::duration_cast<std::chrono::microseconds>(X).count() / 1000.0

#include "timing-utils.h"

std::string getDuration(std::chrono::time_point<std::chrono::high_resolution_clock> start) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    return std::to_string(duration / 1000.0) + "s";
}

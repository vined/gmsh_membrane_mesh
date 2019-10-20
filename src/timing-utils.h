#ifndef GMSH_MEMBRANE_MESH_TIMING_UTILS_H
#define GMSH_MEMBRANE_MESH_TIMING_UTILS_H

#include <chrono>
#include <string>

std::string getDuration(std::chrono::time_point<std::chrono::high_resolution_clock> start);

#endif //GMSH_MEMBRANE_MESH_TIMING_UTILS_H

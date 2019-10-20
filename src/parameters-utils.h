#ifndef GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H
#define GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H

#include <cmath>
#include <string>

const char SEPARATOR = '=';

// Param names
const std::string L = "l";
const std::string H = "h";
const std::string R = "r";

struct parameters {
    double l;
    double h;

    double r;

    parameters() {}

    parameters(
            double _l,
            double _h,
            double _r
    ) {
        l = _l;
        h = _h;
        r = _r;
    }

    std::string toString() {
        std::string str = "Parameters:\n";
        str += L + "=" + std::to_string(l) + ",\n"
               + H + "=" + std::to_string(h) + ",\n"
               + R + "=" + std::to_string(r);

        return str;
    }
};

parameters readParameters(std::string);

#endif //GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H

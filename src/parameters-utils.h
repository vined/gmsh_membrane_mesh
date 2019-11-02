#ifndef GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H
#define GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H

#include <cmath>
//#include <limits>
#include <string>
#include <vector>


const double SCALE = 1e9;

const char KEY_SEPARATOR = '=';
const char VALUES_SEPARATOR = ',';

// Param names
const std::string H_SOLUTION = "h_solution";
const std::string H_MEMBRANE = "h_membrane";
const std::string H_SUB_MEMBRANE = "h_sub_membrane";
const std::string H_HEMHOLTZ = "h_hemholtz";

const std::string V0 = "v0";
const std::string V1 = "v1";
const std::string V2 = "v2";
const std::string V3 = "v3";
const std::string V4 = "v4";
const std::string V5 = "v5";

const std::string DEFECTS_START_LINE = "# -- Defects list";


struct parameters {
    double h_solution;
    double h_membrane;
    double h_sub_membrane;
    double h_hemholtz;

    std::vector<std::pair<double, double>> vertices;
    std::vector<std::pair<std::pair<double, double>, double>> defects;

    parameters() {}

    parameters(
            double _h_solution,
            double _h_membrane,
            double _h_sub_membrane,
            double _h_hemholtz,

            std::vector<std::pair<double, double>> _vertices,
            std::vector<std::pair<std::pair<double, double>, double>> _defects
    ) {
        h_solution = _h_solution;
        h_membrane = _h_membrane;
        h_sub_membrane = _h_sub_membrane;
        h_hemholtz = _h_hemholtz;

        vertices = _vertices;
        defects = _defects;
    }

    void print() {
        std::cout << "Parameters:" << std::endl;
        std::cout << H_SOLUTION << " = " << h_solution << std::endl;
        std::cout << H_MEMBRANE << " = " << h_membrane << std::endl;
        std::cout << H_SUB_MEMBRANE << " = " << h_sub_membrane << std::endl;
        std::cout << H_HEMHOLTZ << " = " << h_hemholtz << std::endl;

        for (auto v: vertices) {
            std::cout << " x=" << v.first << ", y=" << v.second << std::endl;
        }

        std::cout << +"Defects:" << std::endl;
        for (auto d: defects) {
            std::cout << "x=" << d.first.first << ", y=" << d.first.second << ", r=" << d.second << std::endl;

        }
        std::cout << +"Defects count: " << defects.size() << std::endl;
    }
};

parameters readParameters(std::string);

#endif //GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H

#ifndef GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H
#define GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H

#include <cmath>
#include <string>
#include <vector>

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

const std::string DEFECTS = "defects";
const std::string DEFECTS_START_LINE = "# -- Defects list";


struct parameters {
    double h_solution;
    double h_membrane;
    double h_sub_membrane;
    double h_hemholtz;

    std::pair<double, double> v0;
    std::pair<double, double> v1;
    std::pair<double, double> v2;
    std::pair<double, double> v3;
    std::pair<double, double> v4;
    std::pair<double, double> v5;

    std::vector<std::pair<std::pair<double, double>, double>> defects;

    parameters() {}

    parameters(
            double _h_solution,
            double _h_membrane,
            double _h_sub_membrane,
            double _h_hemholtz,

            std::pair<double, double> _v0,
            std::pair<double, double> _v1,
            std::pair<double, double> _v2,
            std::pair<double, double> _v3,
            std::pair<double, double> _v4,
            std::pair<double, double> _v5,

            std::vector<std::pair<std::pair<double, double>, double>> _defects
    ) {
        h_solution = _h_solution;
        h_membrane = _h_membrane;
        h_sub_membrane = _h_sub_membrane;
        h_hemholtz = _h_hemholtz;

        v0 = _v0;
        v1 = _v1;
        v2 = _v2;
        v3 = _v3;
        v4 = _v4;
        v5 = _v5;

        defects = _defects;
    }

    std::string toString() {
        std::string str = "Parameters:\n";
        str += H_SOLUTION + "=" + std::to_string(h_solution) + "\n"
               + H_MEMBRANE + "=" + std::to_string(h_membrane) + "\n"
               + H_SUB_MEMBRANE + "=" + std::to_string(h_sub_membrane) + "\n"
               + H_HEMHOLTZ + "=" + std::to_string(h_hemholtz) + "\n"
               + V0 + "=" + std::to_string(v0.first) + ", " + std::to_string(v0.second) + "\n"
               + V1 + "=" + std::to_string(v1.first) + ", " + std::to_string(v1.second) + "\n"
               + V2 + "=" + std::to_string(v2.first) + ", " + std::to_string(v2.second) + "\n"
               + V3 + "=" + std::to_string(v3.first) + ", " + std::to_string(v3.second) + "\n"
               + V4 + "=" + std::to_string(v4.first) + ", " + std::to_string(v4.second) + "\n"
               + V5 + "=" + std::to_string(v5.first) + ", " + std::to_string(v5.second) + "\n"
               + "Defects_cnt=" + std::to_string(defects.size()) + "\n"
               + "\n";

        return str;
    }
};

parameters readParameters(std::string);

#endif //GMSH_MEMBRANE_MESH_PARAMETERS_UTILS_H

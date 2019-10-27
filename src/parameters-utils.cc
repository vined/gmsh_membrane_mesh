#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "parameters-utils.h"


std::ifstream openFile(std::string fileName) {
    std::ifstream file;
    file.open(fileName);

    if (file.is_open()) {
        return file;
    }

    std::cout << "Failed to open file: " << fileName << std::endl;
    throw 200;
}


bool isCommentOrEmpty(std::string line) {
    return line.empty() || line[0] == '#';
}

std::pair<std::string, std::string> splitBy(std::string str, char sep) {
    std::size_t sep_pos = str.find(sep);

    if (sep_pos != std::string::npos) {
        return {
                str.substr(0, sep_pos),
                str.substr(sep_pos + 1, str.size()),
        };
    }
    return {};
};

bool startsWith(std::string str, std::string start) {
    return !str.empty() && str.find(start) == 0;
}


double toDouble(std::string val) {
    return std::stod(val) * SCALE;
}

std::pair<std::string, std::string> readParam(std::string line) {
    return splitBy(line, KEY_SEPARATOR);
}

std::string getParamByName(std::vector<std::pair<std::string, std::string>> params, std::string name) {
    for (std::pair<std::string, std::string> param : params) {
        if (param.first == name && !param.second.empty()) {
            return param.second;
        }
    }

    std::cout << "Failed to read value for parameter: " << name << std::endl;
    throw 201;
};


std::pair<double, double> parsePoint(std::string pointStr) {
    auto strPair = splitBy(pointStr, VALUES_SEPARATOR);

    return {
            toDouble(strPair.first),
            toDouble(strPair.second)
    };
};

std::pair<std::pair<double, double>, double> parseCoordAndRadius(std::string pointStr) {
    auto x_rest = splitBy(pointStr, VALUES_SEPARATOR);
    auto y_r = splitBy(x_rest.second, VALUES_SEPARATOR);

    return
            {
                    {
                            toDouble(x_rest.first),
                            toDouble(y_r.first)
                    },
                    toDouble(y_r.second)
            };
};

std::vector<std::pair<std::pair<double, double>, double>> parseDefects(std::vector<std::string> defectStrList) {
    std::vector<std::pair<std::pair<double, double>, double>> defects;

    for (std::string str : defectStrList) {
        defects.push_back(parseCoordAndRadius(str));
    }

    return defects;
};


parameters parseParameters(std::vector<std::pair<std::string, std::string>> params, std::vector<std::string> defects) {
    return parameters(
            toDouble(getParamByName(params, H_SOLUTION)),
            toDouble(getParamByName(params, H_MEMBRANE)),
            toDouble(getParamByName(params, H_SUB_MEMBRANE)),
            toDouble(getParamByName(params, H_HEMHOLTZ)),

            parsePoint(getParamByName(params, V0)),
            parsePoint(getParamByName(params, V1)),
            parsePoint(getParamByName(params, V2)),
            parsePoint(getParamByName(params, V3)),
            parsePoint(getParamByName(params, V4)),
            parsePoint(getParamByName(params, V5)),

            parseDefects(defects)
    );
}

parameters readParameters(std::string fileName) {

    std::ifstream paramsFile = openFile(fileName);
    std::string line;
    std::vector<std::pair<std::string, std::string>> params;
    std::vector<std::string> defects;

    while (getline(paramsFile, line)) {
        if (!isCommentOrEmpty(line)) {
            params.push_back(readParam(line));
        } else if (startsWith(line, DEFECTS_START_LINE)) {
            break;
        }
    }

    while (getline(paramsFile, line)) {
        if (!isCommentOrEmpty(line)) {
            defects.push_back(line);
        }
    }

    return parseParameters(params, defects);
}

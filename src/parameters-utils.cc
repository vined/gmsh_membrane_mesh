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


std::pair<std::string, std::string> readParam(std::string line) {
    return splitBy(line, KEY_SEPARATOR);
}

std::string getParamByName(std::vector<std::pair<std::string, std::string>> params, std::string name) {
    std::cout << "Getting param for name: " << name << std::endl;
    for (std::pair<std::string, std::string> param : params) {
        if (param.first == name && !param.second.empty()) {
            return param.second;
        }
    }

    std::cout << "Failed to read value for parameter: " << name << std::endl;
    throw 201;
};


std::pair<double, double> parsePoint(std::string pointStr) {
    std::cout << "Parse point: " << pointStr << std::endl;
    auto strPair = splitBy(pointStr, VALUES_SEPARATOR);

    std::cout << "Parsed point values: " << strPair.first << " and " << strPair.first << std::endl;
    return {
            std::stof(strPair.first),
            std::stof(strPair.second)
    };
};

std::pair<std::pair<double, double>, double> parseCoordAndRadius(std::string pointStr) {
    std::cout << "Parse coords: " << pointStr << std::endl;
    auto x_rest = splitBy(pointStr, VALUES_SEPARATOR);
    auto y_r = splitBy(x_rest.second, VALUES_SEPARATOR);

    return
            {
                    {
                            std::stof(x_rest.first),
                            std::stof(y_r.first)
                    },
                    std::stof(y_r.second)
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
            std::stof(getParamByName(params, H_SOLUTION)),
            std::stof(getParamByName(params, H_MEMBRANE)),
            std::stof(getParamByName(params, H_SUB_MEMBRANE)),
            std::stof(getParamByName(params, H_HEMHOLTZ)),

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
            std::cout << "Push line to params: " << line << std::endl;
            params.push_back(readParam(line));
        } else if (startsWith(line, DEFECTS_START_LINE)) {
            break;
        }
    }

    while (getline(paramsFile, line)) {
        if (!isCommentOrEmpty(line)) {
            std::cout << "Push line to defects: " << line << std::endl;
            defects.push_back(line);
        }
    }

    return parseParameters(params, defects);
}

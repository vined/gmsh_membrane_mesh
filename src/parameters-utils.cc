#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <vector>
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

std::pair<std::string, std::string> readParam(std::string line) {

    std::size_t sep_pos = line.find(SEPARATOR);

    if (sep_pos != std::string::npos) {
        return {
            line.substr(0, sep_pos),
            line.substr(sep_pos+1, line.size()),
        };
    }
    return {};
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

parameters parseParameters(std::vector<std::pair<std::string, std::string>> params) {
    return parameters(
            std::stof(getParamByName(params, L)),
            std::stof(getParamByName(params, H)),
            std::stof(getParamByName(params, R))
    );
}

parameters readParameters(std::string fileName) {

    std::ifstream paramsFile = openFile(fileName);
    std::string line;
    std::vector<std::pair<std::string, std::string>> params;

    while (getline(paramsFile, line)) {

        if (!isCommentOrEmpty(line)) {
            params.push_back(readParam(line));
        }
    }

    return parseParameters(params);
}

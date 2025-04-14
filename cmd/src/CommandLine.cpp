//
// Created by stanl on 25-4-14.
//

#include "CommandLine.h"

#include <iostream>
#include <ostream>
#include <sstream>

CommandLine & CommandLine::getInstance() {
    static CommandLine instance;
    return instance;
}

void CommandLine::start() {
    std::cout << "Welcome to Data4Sql!" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::string input;
    int status = 0;
    std::vector<std::string> result;
    while (true) {
        std::cout << "Data4Sql > ";
        std::getline(std::cin, input);
        if (preProcess(input) == -1) {
            std::cout << "ERROR: No command Given." << std::endl;
        }
        status = tokenize(input, result);
        if (status == -1) continue;
        if (status == 0) {
            // finish
            for (int i = 0; i < result.size(); i++) {
                std::cout << result[i] << std::endl;
            }
            auto message = _parser.parse(result);
            std::cout << message << std::endl;
            result.clear();
        }
    }
}

inline int CommandLine::preProcess(std::string &str) {
    if(str.empty()) return -1;
    // 删去前后多余空格 (trim)
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    return 0;
}

// 向result追加
int CommandLine::tokenize(std::string &str, std::vector<std::string> &result) {
    int ret = -1;
    if (str.back() == ';') {
        str.pop_back();
        ret = 0;
    }
    std::stringstream ss(str);
    std::string token;
    while (ss >> token) {
        result.push_back(token);
    }
    return ret;
}

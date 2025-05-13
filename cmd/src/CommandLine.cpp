//
// Created by stanl on 25-4-14.
//

#include "CommandLine.h"

#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "easylogging++.h"
#include "json.hpp"

INITIALIZE_EASYLOGGINGPP

CommandLine & CommandLine::getInstance() {
    static CommandLine instance;
    return instance;
}

void CommandLine::start() {
    // Initialize Logger
    el::Configurations conf("conf/log.conf");
    el::Loggers::reconfigureLogger("default", conf);
    el::Loggers::reconfigureAllLoggers(conf);

    // Initialize User
    std::ifstream i("conf/users.json");
    nlohmann::json users;
    i >> users;
    LOG(INFO) << users;

    std::cout << "Please enter username and password:" << std::endl;
    std::string username, password;
    while (true) {
        std::cin >> username >> password;
        std::string t = users[username]["password"];
        if (t == password) {
            LOG(INFO) << "Successfully logged as " << username;
            std::cout << "Successfully logged as " << username << std::endl;
            this->_username = username;
            _parser._username = username;
            _parser._user_json = users;
            std::cin.clear();
            break;
        }
        std::cout << "User " << username << " does not exist or wrong password!" << std::endl;
    }

    std::cout << "Welcome to Data4Sql!" << std::endl;
    std::cout << "--------------------" << std::endl;
    LOG(INFO) << "CommandLine::start()";
    std::string input;
    int status = 0;
    std::vector<std::string> result;
    while (true) {
        std::cout << "Data4Sql > ";
        std::getline(std::cin, input);
        if (preProcess(input) == -1) {
            std::cout << "ERROR: No command Given." << std::endl;
            LOG(ERROR) << "ERROR: No command Given." << std::endl;
            continue;
        }
        LOG(INFO) << "Received command: " << input;
        status = tokenize(input, result);
        if (status == -1) continue;
        if (status == 0) {
            // finish
            if (result[0] == "EXECUTE") {
                for (std::string temp : users[this->_username]["permissions"]) {
                    if (temp == "ROOT") {
                        ReadSqlBatch(result[1]);
                        goto NEXT;
                    }
                }
                std::cout << "ERROR: Permission denied." << std::endl;
                LOG(ERROR) << "ERROR: Permission denied." << std::endl;
                continue;
            }
            auto message = _parser.parse(result);
            std::cout << message << std::endl;
            LOG(INFO) << message;
            NEXT:
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


int CommandLine::processSQL(const std::string& input, std::vector<std::string>& tokens) {
    std::string processed = input;
    if (preProcess(processed) == -1) return -1;
    return tokenize(processed, tokens);
}

int CommandLine::ReadSqlBatch(std::string& file_name) {
    int ret = 0;
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_name << std::endl;
        return -2;
    }
    std::string line;
    std::vector<std::string> result;
    while (std::getline(file, line)) {
        preProcess(line);
        auto status = tokenize(line, result);
        if (status == -1) continue;
        if (status == 0) {
            auto message = _parser.parse(result);
            std::cout << message << std::endl;
            result.clear();
        }
    }
    file.close();
    return ret;
}

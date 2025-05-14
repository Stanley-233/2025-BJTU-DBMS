//
// Created by stanl on 25-4-11.
//

#ifndef PARSER_H
#define PARSER_H

#include <CoreProcess.h>
#include <string>
#include <vector>

#include "json.hpp"

class Parser {
    const std::string WIP = "ERROR: Not Implemented.";
public:
    std::string _username;
    nlohmann::json _user_json;
    CoreProcess _coreProcess = CoreProcess::getInstance();
    Parser() = default;
    std::string parse(const std::vector<std::string>& input);
private:
    // DATABASE
    std::string createDatabase(const std::vector<std::string>& input);
    std::string dropDatabase(const std::vector<std::string>& input);
    std::string useDatabase(const std::vector<std::string>& input);

    // TABLE
    std::string createTable(const std::vector<std::string>& input);
    std::string dropTable(const std::vector<std::string>& input);
    std::string alterTable(const std::vector<std::string>& input);
    std::string insertIntoTable(const std::vector<std::string>& input);
    std::string DeleteFromTable(const std::vector<std::string>& input);
    std::string UpdateRecord(const std::vector<std::string>& input);

    std::string SelectFromTable(const std::vector<std::string>& input);

    // USER
    std::string GrantUserPermission(const std::string& username, const std::string& permission);
    std::string RevokeUserPermission(const std::string& username, const std::string& permission);
};



#endif //PARSER_H

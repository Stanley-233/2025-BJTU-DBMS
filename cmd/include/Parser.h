//
// Created by stanl on 25-4-11.
//

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

class Parser {
    constexpr std::string WIP = "ERROR: Not Implemented.";
public:
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
};



#endif //PARSER_H

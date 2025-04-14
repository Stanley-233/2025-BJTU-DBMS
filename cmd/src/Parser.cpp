//
// Created by stanl on 25-4-11.
//

#include "Parser.h"

std::string Parser::parse(const std::vector<std::string> &input) {
    std::string output;
    if (input.size() <= 0) return "ERROR: No Command Given.";
    // DATABASE
    if (input[1] == "DATABASE") {
        if (input[0] == "CREATE") {
            output = createDatabase(input);
        } else if (input[0] == "DROP") {
            output = dropDatabase(input);
        } else if (input[0] == "USE") {
            output = useDatabase(input);
        }
    }
    // TABLE
    if (input[1] == "TABLE") {
        if (input[0] == "CREATE") {
            output = createTable(input);
        } else if (input[0] == "DROP") {
            output = dropTable(input);
        } else if (input[0] == "ALTER") {
            output = alterTable(input);
        }
    }
    return output;
}

std::string Parser::createDatabase(const std::vector<std::string> &input) {
    return WIP;
}

std::string Parser::dropDatabase(const std::vector<std::string> &input) {
    return WIP;
}

std::string Parser::useDatabase(const std::vector<std::string> &input) {
    return WIP;
}

std::string Parser::createTable(const std::vector<std::string> &input) {
    return WIP;
}

std::string Parser::dropTable(const std::vector<std::string> &input) {
    return WIP;
}

std::string Parser::alterTable(const std::vector<std::string> &input) {
    return WIP;
}

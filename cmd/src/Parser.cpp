//
// Created by stanl on 25-4-11.
//

#include "Parser.h"
#include "DataTypes.h"

#include <map>

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
    auto databaseName = input[2];
    return WIP;
}

std::string Parser::dropDatabase(const std::vector<std::string> &input) {
    auto databaseName = input[2];
    return WIP;
}

std::string Parser::useDatabase(const std::vector<std::string> &input) {
    auto databaseName = input[2];
    return WIP;
}

std::string Parser::createTable(const std::vector<std::string> &input) {
    if (input.size() % 2 != 1 || input.size() < 5) return "ERROR: Wrong Syntax.";
    std::string output = WIP;
    auto tableName = input[2];
    std::map<std::string, std::string> colTypes;
    std::vector<std::string> colNames;
    for (int i = 3; i < input.size(); ++i) {
        auto colName = input[i];
        auto colType = input[i + 1];
        if (colType != "INTEGER" && colType != "INT" && colType != "TEXT" && colType != "DECIMAL") {
            return "ERROR: Wrong Datatype.";
        }
        colNames.emplace_back(colName);
        colTypes.insert(colName, colType);
    }
    return output;
}

std::string Parser::dropTable(const std::vector<std::string> &input) {
    auto tableName = input[2];
    return WIP;
}

std::string Parser::alterTable(const std::vector<std::string> &input) {
    auto tableName = input[2];
    auto command = input[3];
    if (command == "ADD") {

    } else if (command == "DROP") {

    } else if (command == "MODIFY") {

    }
    return WIP;
}

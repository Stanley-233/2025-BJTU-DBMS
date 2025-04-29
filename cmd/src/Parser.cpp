//
// Created by stanl on 25-4-11.
//

#include "Parser.h"
#include "DataTypes.h"

#include <unordered_map>

#include "Table.h"

std::string Parser::parse(const std::vector<std::string> &input) {
    std::string output = "ERROR: Invalid syntax";
    if (input.empty()) return "ERROR: No Command Given.";
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
    if (input[0] == "INSERT") {
        if (input.size() < 4 || input[1] != "INTO") {
            output = "ERROR: Invalid syntax.";
        } else {
            output = insertIntoTable(input);
        }
    }
    if (input[0] == "DELETE") {
        if (input.size() < 3 || input[1] != "FROM") {
            output = "ERROR: Invalid syntax.";
        } else {
            output = DeleteFromTable(input);
        }
    }
    if (input[0] == "UPDATE") {
        if (input.size() < 6 || input[2] != "SET") {
            output = "ERROR: Invalid syntax.";
        } else {
            output = UpdateRecord(input);
        }
    }
    if (input[0] == "SELECT") {
        if (input.size() < 4) {
            output = "ERROR: Invalid syntax.";
        } else {
            output = SelectFromTable(input);
        }
    }
    return output;
}

std::string Parser::createDatabase(const std::vector<std::string> &input) {
    const auto& databaseName = input[2];
    return _coreProcess.createDatabase(databaseName);
}

std::string Parser::dropDatabase(const std::vector<std::string> &input) {
    const auto& databaseName = input[2];
    return _coreProcess.dropDatabase(databaseName);
}

std::string Parser::useDatabase(const std::vector<std::string> &input) {
    const auto& databaseName = input[2];
    return _coreProcess.useDatabase(databaseName);
}

std::string Parser::createTable(const std::vector<std::string> &input) {
    if (input.size() % 2 != 1 || input.size() < 5) return "ERROR: Invalid syntax.";
    std::string output = WIP;
    const auto& tableName = input[2];
    std::unordered_map<std::string, int> colTypes;
    std::vector<std::string> colNames;
    for (int i = 3; i < input.size(); i+=2) {
        std::string colName = input[i];
        const std::string& colType = input[i + 1];\
        if (colType == "INTEGER" || colType == "INT") {
            colTypes.emplace(colName, 1);
        } else if (colType == "TEXT") {
            colTypes.emplace(colName, 0);
        } else if (colType == "DECIMAL") {
            colTypes.emplace(colName, 2);
        } else {
            return "ERROR: Wrong Datatype. Supported types: INTEGER, INT, TEXT, DECIMAL.";
        }
        colNames.emplace_back(colName);
    }
    output = _coreProcess.createTable(tableName, colNames, colTypes);
    return output;
}

std::string Parser::dropTable(const std::vector<std::string> &input) {
    const auto& tableName = input[2];
    return _coreProcess.dropTable(tableName);
}

std::string Parser::alterTable(const std::vector<std::string> &input) {
    std::string output;
    const auto& tableName = input[2];
    const auto& command = input[3];
    if (command == "ADD") {
        if (input.size() != 6) return "ERROR: Invalid syntax.";
        output = _coreProcess.alterTableAdd(tableName, input[4], input[5]);
    } else if (command == "DROP") {
        if (input.size() != 5) return "ERROR: Invalid syntax.";
        output = _coreProcess.alterTableDrop(tableName, input[4]);
    } else if (command == "MODIFY") {
        if (input.size() != 6) return "ERROR: Invalid syntax.";
        output = _coreProcess.alterTableModify(tableName, input[4], input[5]);
    } else if (command == "RENAME") {
        if (input.size() != 5) return "ERROR: Invalid syntax.";
        output = _coreProcess.renameTable(tableName, input[4]);
    }
    return output;
}

std::string Parser::insertIntoTable(const std::vector<std::string> &input) {
    const auto& tableName = input[2];
    std::vector<std::string> colNames;
    int i = 0;
    for (i = 3; i < input.size(); i++) {
        if (input[i] == "VALUES") break;
        colNames.emplace_back(input[i]);
    }
    if (input.size() != i + 1 + colNames.size()) {
        return "ERROR: Invalid syntax.";
    }
    std::unordered_map<std::string, std::string> colMap;
    int k = 0;
    for (int j = i + 1; j < input.size(); j++) {
        colMap.emplace(colNames[k], input[j]);
        k++;
    }
    std::string output = _coreProcess.insertIntoTable(tableName, colMap);
    return output;
}

std::string Parser::DeleteFromTable(const std::vector<std::string> &input) {
    const auto& tableName = input[2];
    std::unordered_map<std::string, std::string> conditions;
    if (input.size() > 3) {
        if (input[3] != "WHERE" || input.size() < 7 || (input.size()-4)%3!=0) return "ERROR: Invalid syntax.";
        for (int i = 4; i < input.size(); i++) {
            if (input[i+1] != "=") return "ERROR: Invalid syntax.";
            conditions.emplace(input[i], input[i+2]);
        }
    }
    std::string output = _coreProcess.DeleteFromTable(tableName, conditions);
    return output;
}

std::string Parser::UpdateRecord(const std::vector<std::string> &input) {
    const auto& tableName = input[2];
    std::unordered_map<std::string, std::string> values;
    std::unordered_map<std::string, std::string> conditions;
    int i = 3;
    for (; i < input.size(); i+=3) {
        if (input[i] == "WHERE") break;
        if (input[i+1] != "=") return "ERROR: Invalid syntax.";
        values.emplace(input[i], input[i+2]);
    }
    // 是 WHERE
    for (++i; i < input.size(); i+=3) {
        if (input[i+1] != "=") return "ERROR: Invalid syntax.";
        conditions.emplace(input[i], input[i+2]);
    }
    std::string output = _coreProcess.UpdateTableRecord(tableName, values, conditions);
    return output;
}

std::string Parser::SelectFromTable(const std::vector<std::string> &input) {
    int i = 1;
    std::vector<std::string> colNames;
    for (; i < input.size(); i++) {
        if (input[i] == "FROM") break;
        colNames.emplace_back(input[i]);
    }
    const auto& tableName = input[++i];
    if (input[++i] != "WHERE") return "ERROR: Invalid syntax.";
    ++i;
    std::unordered_map<std::string, std::string> conditions;
    for (; i < input.size(); i+=3) {
        if (input[i+1] != "=") return "ERROR: Invalid syntax.";
        conditions.emplace(input[i], input[i+2]);
    }
    if (colNames.empty()) return "ERROR: Invalid syntax.";
    std::string output;
    if (colNames[0] == "*") {
        output = _coreProcess.SelectAllFromTable(tableName, conditions);
    } else {
        output = _coreProcess.SelectColFromTable(tableName, colNames, conditions);
    }
    return output;
}

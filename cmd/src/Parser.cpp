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
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (temp == "ROOT" || temp == "DATABASE") {
                if (input[0] == "CREATE") {
                    output = createDatabase(input);
                    goto RET;
                }
                if (input[0] == "DROP") {
                    output = dropDatabase(input);
                    goto RET;
                }
                if (input[0] == "USE") {
                    output = useDatabase(input);
                    goto RET;
                }
            }
        }
    }
    // TABLE
    if (input[1] == "TABLE") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input[0] == "CREATE") {
                if (temp == "CREATE_TABLE" || temp == "ROOT") {
                    output = createTable(input);
                    goto RET;
                }
            } else if (input[0] == "DROP") {
                if (temp == "DROP_TABLE" || temp == "ROOT") {
                    output = dropTable(input);
                    goto RET;
                }
            } else if (input[0] == "ALTER") {
                if (temp == "ALTER_TABLE" || temp == "ROOT") {
                    output = alterTable(input);
                    goto RET;
                }
            }
        }
        output = "ERROR: Permission denied.";
    }
    if (input[0] == "INSERT") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input.size() < 4 || input[1] != "INTO") {
                output = "ERROR: Invalid syntax.";
                break;
            }
            if (temp == "INSERT" || temp == "ROOT") {
                output = insertIntoTable(input);
                goto RET;
            }
        }
    }
    if (input[0] == "DELETE") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input.size() < 3 || input[1] != "FROM") {
                output = "ERROR: Invalid syntax.";
                break;
            }
            if (temp == "DELETE" || temp == "ROOT") {
                output = DeleteFromTable(input);
                goto RET;
            }
        }
    }
    if (input[0] == "UPDATE") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input.size() < 6 || input[2] != "SET") {
                output = "ERROR: Invalid syntax.";
                break;
            }
            if (temp == "UPDATE" || temp == "ROOT") {
                output = UpdateRecord(input);
                goto RET;
            }
        }
    }
    if (input[0] == "SELECT") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input.size() < 4) {
                output = "ERROR: Invalid syntax.";
                break;
            }
            if (temp == "SELECT" || temp == "ROOT") {
                output = SelectFromTable(input);
                goto RET;
            }
        }
    }
    if (input[0] == "GRANT") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input.size() < 4 || input[2] != "ON") {
                output = "ERROR: Invalid syntax.";
                break;
            }
            if (temp == "ROOT") {
                output = GrantUserPermission(input[3], input[1]);
                goto RET;
            }
        }
    }
    if (input[0] == "REVOKE") {
        for (std::string temp : _user_json[_username]["permissions"]) {
            if (input.size() < 4 || input[2] != "ON") {
                output = "ERROR: Invalid syntax.";
                break;
            }
            if (temp == "ROOT") {
                output = RevokeUserPermission(input[3], input[1]);
                goto RET;
            }
        }
    }
    if (input[0] == "CREATE" && input[1] == "USER") {
        const auto& uname = input[2];
        const auto& password = input[3];
        if (_user_json.contains(uname)) {
            output = "User '" + uname + "' already exists";
        }
        _user_json[uname] = {
            {"password", password},
            {"permissions", nlohmann::json::array()}  // 初始权限为空
        };
        std::ofstream outfile("conf/users.json",std::ios::trunc);
        outfile << _user_json << std::flush;
        outfile.close();
    }
    RET:
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
        for (int i = 4; i < input.size(); i+=3) {
            if (input[i+1] != "=") return "ERROR: Invalid syntax.";
            conditions.emplace(input[i], input[i+2]);
        }
    }
    std::string output = _coreProcess.DeleteFromTable(tableName, conditions);
    return output;
}

std::string Parser::UpdateRecord(const std::vector<std::string> &input) {
    const auto& tableName = input[1];
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
    std::string output = _coreProcess.UpdateTableRecord(tableName, conditions, values);
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
    std::unordered_map<std::string, std::string> conditions;
    if (input.size() > 4) {
        if (input[++i] != "WHERE") return "ERROR: Invalid syntax.";
        ++i;
        for (; i < input.size(); i+=3) {
            if (input[i] == "JOIN") break;
            if (input[i+1] != "=") return "ERROR: Invalid syntax.";
            conditions.emplace(input[i], input[i+2]);
        }
        if (colNames.empty()) return "ERROR: Invalid syntax.";
    }
    // JOIN tablename ON Col1 = Col2
    ++i;
    std::string join_table_name, this_col_name, other_col_name;
    if (i < input.size()) {
        for (; i < input.size(); i+=5) {
            join_table_name = input[i];
            if (input[i+1] != "ON") return "ERROR: Invalid syntax.";
            this_col_name = input[i+2];
            if (input[i+3] != "=") return "ERROR: Invalid syntax.";
            other_col_name = input[i+4];
        }
    }
    std::string output;
    if (colNames[0] == "*") {
        output = _coreProcess.SelectAllFromTable(tableName, conditions);
    } else if (!join_table_name.empty()) {
        output = _coreProcess.SelectFromTabelJoin(tableName, colNames, conditions,
            join_table_name, this_col_name, other_col_name);
    } else {
        output = _coreProcess.SelectColFromTable(tableName, colNames, conditions);
    }
    return output;
}

std::string Parser::GrantUserPermission(const std::string &username, const std::string &permission) {
    if (!_user_json.contains(username)) {
        return "ERROR: User not exists";
    }
    auto& permissions = _user_json[username]["permissions"];
    if (std::find(permissions.begin(), permissions.end(), permission) != permissions.end()) {
        return "ERROR: Permission already granted";
    }
    permissions.push_back(permission);
    std::ofstream outfile("conf/users.json",std::ios::trunc);
    outfile << _user_json << std::flush;
    outfile.close();
    return "Permission granted to " + username;
}

std::string Parser::RevokeUserPermission(const std::string &username, const std::string &permission) {
    if (!_user_json.contains(username)) {
        return "ERROR: User not exists";
    }
    auto& permissions = _user_json[username]["permissions"];
    auto it = std::find(permissions.begin(), permissions.end(), permission);
    if (it == permissions.end()) {
        return "ERROR: Permission not found.";
    }
    permissions.erase(it);
    std::ofstream outfile("conf/users.json",std::ios::trunc);
    outfile << _user_json << std::flush;
    outfile.close();
    return "Permission revoked from " + username;
}

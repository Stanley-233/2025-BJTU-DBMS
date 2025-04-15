//
// Created by stanl on 25-4-15.
//

#include "CoreProcess.h"

CoreProcess & CoreProcess::getInstance() {
    static CoreProcess instance;
    return instance;
}

std::string CoreProcess::createDatabase(const std::string &name) {
    return "ERROR:WIP";
}

std::string CoreProcess::dropDatabase(const std::string &name) {
    return "ERROR:WIP";
}

std::string CoreProcess::useDatabase(const std::string &name) {
    return "ERROR:WIP";
}

std::string CoreProcess::createTable(const std::string &table_name, const std::vector<std::string> &columns_name,
    const std::map<std::string, std::string> &colTypes) {
    return "ERROR:WIP";
}

std::string CoreProcess::dropTable(const std::string &table_name) {
    return "ERROR:WIP";
}

std::string CoreProcess::alterTableAdd(const std::string &table_name, const std::string &col_name,
    const std::string &col_type) {
    return "ERROR:WIP";
}

std::string CoreProcess::alterTableDrop(const std::string &table_name, const std::string &col_name) {
    return "ERROR:WIP";
}

std::string CoreProcess::alterTableModify(const std::string &table_name, const std::string &col_name,
    const std::string &col_type) {
    return "ERROR:WIP";
}

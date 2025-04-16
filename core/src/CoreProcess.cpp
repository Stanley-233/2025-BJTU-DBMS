//
// Created by stanl on 25-4-15.
//
#include "CoreProcess.h"

#include <DataTypes.h>

#include <Table.h>

CoreProcess & CoreProcess::getInstance() {
    static CoreProcess instance;
    return instance;
}

std::string CoreProcess::createDatabase(const std::string &name) const {
    const fs::path dirPath = SYS_PATH / fs::path(name);
    if (exists(dirPath) && is_directory(dirPath)) {
        return "ERROR: Database Already Exists.";
    }
    try {
        if (create_directories(dirPath)) {
            return "Database created successfully.";
        }
        return "ERROR: Failed to create database directory.";
    } catch (const fs::filesystem_error& e) {
        return "ERROR: " + std::string(e.what());
    }
}

std::string CoreProcess::dropDatabase(const std::string &name) const {
    const fs::path dirPath = SYS_PATH / fs::path(name);
    if (!exists(dirPath) || !is_directory(dirPath)) {
        return "ERROR: Database not exist.";
    }
    try {
        remove_all(dirPath);
        return "Database dropped successfully.";
    } catch (const fs::filesystem_error& e) {
        return "ERROR: " + std::string(e.what());
    }
}

std::string CoreProcess::useDatabase(const std::string &name) {
    const fs::path dbPath = SYS_PATH / fs::path(name);
    if (!exists(dbPath) || !is_directory(dbPath)) {
        return "ERROR: Database not exist.";
    }
    currentDbName = name;
    return "Database changed to " + name + " successfully.";
}

std::string CoreProcess::createTable(const std::string &table_name,
    const std::vector<std::string> &columns_name,
    const std::unordered_map<std::string, TypeHandler> &colTypes) const {
    if (currentDbName == "") return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    Table::create_table(SYS_PATH + '/' + currentDbName, table_name, colTypes, columns_name);
    return "Successfully created table.";
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

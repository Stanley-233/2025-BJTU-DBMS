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
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    Table::create_table(SYS_PATH + '/' + currentDbName, table_name, colTypes, columns_name);
    return "Successfully created table.";
}

std::string CoreProcess::dropTable(const std::string &table_name) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    t.drop_table();
    free(&t);
    return "Successfully drop table.";
}

std::string CoreProcess::alterTableAdd(const std::string &table_name, const std::string &col_name,
    const std::string &col_type) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    int type = -1;
    switch (col_type) {
        case "TEXT":
            type = 0;
            break;
        case "INTEGER":
            type = 1;
            break;
        case "DECIMAL":
            type = 2;
            break;
        default:
            return "ERROR: " + col_type + " is not a valid column type.";
    }
    t.alter_table_add_column(col_name, type);
    free(&t);
    return "Successfully altered table.";
}

std::string CoreProcess::alterTableDrop(const std::string &table_name, const std::string &col_name) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    t.alter_table_drop_column(col_name);
    free(&t);
    return "Successfully drop column.";
}

std::string CoreProcess::alterTableModify(const std::string &table_name, const std::string &col_name,
    const std::string &col_type) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    int type = -1;
    switch (col_type) {
        case "TEXT":
            type = 0;
        break;
        case "INTEGER":
            type = 1;
        break;
        case "DECIMAL":
            type = 2;
        break;
        default:
            return "ERROR: " + col_type + " is not a valid column type.";
    }
    t.alter_table_modify(col_name, type);
    free(&t);
    return "Successfully drop column.";
}

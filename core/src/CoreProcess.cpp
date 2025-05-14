//
// Created by stanl on 25-4-15.
//
#include "../include/CoreProcess.h"

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
    const std::unordered_map<std::string, int> &colTypes) const {
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
    if (col_type == "TEXT") {
        type = 0;
    } else if (col_type == "INTEGER") {
        type = 1;
    } else if (col_type == "DECIMAL") {
        type = 2;
    } else {
        return "ERROR: " + col_type + " is not a valid column type.";
    }
    t.alter_table_add_column(col_name, type);
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
    if (col_type == "TEXT") {
        type = 0;
    } else if (col_type == "INTEGER") {
        type = 1;
    } else if (col_type == "DECIMAL") {
        type = 2;
    } else {
        return "ERROR: " + col_type + " is not a valid column type.";
    }
    t.alter_table_modify(col_name, type);
    return "Successfully drop column.";
}

std::string CoreProcess::renameTable(const std::string &old_table_name, const std::string &new_table_name) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + old_table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    t.alter_table_rename_table(new_table_name);
    return "Successfully renamed table.";
}

std::string CoreProcess::insertIntoTable(const std::string &table_name,
std::unordered_map<std::string, std::string> &colMap) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    switch (t.insert_row(colMap)) {
        case 0:
            return "Successfully inserted into table " + table_name + " .";
        case 1:
            return "Column number invalid. ";
        case 2:
            return "Column name didn't exist. ";
        default:
            return "Super Big Error!!!";
    }
}

std::string CoreProcess::DeleteFromTable(const std::string &table_name,
    std::unordered_map<std::string, std::string> &conditions) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    int ret = t.delete_rows(conditions);
    return ret + " records deleted from " + table_name + " .";
}

std::string CoreProcess::UpdateTableRecord(const std::string &table_name,
    std::unordered_map<std::string, std::string> &col_values,
    std::unordered_map<std::string, std::string> &conditions) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    int ret = t.update_rows(col_values, conditions);
    return ret + " records updated from " + table_name + " .";
}

std::string CoreProcess::SelectAllFromTable(const std::string &table_name,
    std::unordered_map<std::string, std::string> &conditions) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    std::vector<std::string> rows(0);
    std::string output = t.select_rows(conditions, rows);
    return output;
}

std::string CoreProcess::SelectColFromTable(const std::string &table_name, std::vector<std::string> &columns,
    std::unordered_map<std::string, std::string> &conditions) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    Table t(path);
    std::string output = t.select_rows(conditions, columns);
    return output;
}

std::string CoreProcess::SelectFromTabelJoin(const std::string &table_name, std::vector<std::string> &columns,
    std::unordered_map<std::string, std::string> &conditions, const std::string &join_table_name,
    const std::string &this_join_column_name, const std::string &other_join_column_name) {
    if (currentDbName.empty()) return "ERROR: Require Database. Use \"USE DATABASE db_name\" first.";
    auto path = SYS_PATH + '/' + currentDbName + '/' + table_name;
    if (!fs::exists(path + ".csv")) {
        return "ERROR: Table not exist.";
    }
    // TODO
    Table t(path);
    std::vector<std::string> rows(0);
    std::string output = t.select_rows(conditions, rows);
    return output;
}

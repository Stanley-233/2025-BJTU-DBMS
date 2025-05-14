//
// Created by stanl on 25-4-15.
//

#ifndef COREPROCESS_H
#define COREPROCESS_H

#include <complex.h>
#include <DataTypes.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class CoreProcess {
public:
    // 获取单例对象
    static CoreProcess& getInstance();

    // ---TABLE---
    [[nodiscard]] std::string createDatabase(const std::string& name) const;
    [[nodiscard]] std::string dropDatabase(const std::string& name) const;
    std::string useDatabase(const std::string& name);

    // TABLE
    [[nodiscard]] std::string createTable(const std::string &table_name,
                            const std::vector<std::string> &columns_name,
                            const std::unordered_map<std::string, int> &colTypes) const;
    std::string dropTable(const std::string& table_name);
    std::string alterTableAdd(const std::string& table_name, const std::string& col_name, const std::string& col_type);
    std::string alterTableDrop(const std::string& table_name, const std::string& col_name);
    std::string alterTableModify(const std::string& table_name, const std::string& col_name, const std::string& col_type);
    std::string renameTable(const std::string& old_table_name, const std::string& new_table_name);
    // key = colname, value = value
    std::string insertIntoTable(const std::string &table_name, std::unordered_map<std::string, std::string> &colMap);
    // key = colname, value = condition value
    std::string DeleteFromTable(const std::string& table_name, std::unordered_map<std::string, std::string> &conditions);
    // col_values:(colname, new_value)
    std::string UpdateTableRecord(const std::string& table_name,
        std::unordered_map<std::string, std::string> &col_values,
        std::unordered_map<std::string, std::string> &conditions);

    // Query
    std::string SelectAllFromTable(const std::string& table_name,
        std::unordered_map<std::string, std::string> &conditions);
    std::string SelectColFromTable(const std::string& table_name,
        std::vector<std::string> &columns,
        std::unordered_map<std::string, std::string> &conditions);
    std::string SelectFromTabelJoin(const std::string& table_name,
        std::vector<std::string> &columns,
        std::unordered_map<std::string, std::string> &conditions,
        const std::string& join_table_name,
        std::string& this_join_column_name,
        std::string& other_join_column_name);

    // Commit
    std::string Commit(std::string& table_name);
    std::string Rollback(std::string& table_name);

private:
    std::string currentDbName;
    const std::string SYS_PATH = "system";
};



#endif //COREPROCESS_H

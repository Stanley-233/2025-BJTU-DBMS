//
// Created by stanl on 25-4-15.
//

#ifndef COREPROCESS_H
#define COREPROCESS_H

#include <string>
#include <vector>
#include <map>

class CoreProcess {
public:
    // 获取单例对象
    static CoreProcess& getInstance();

    // ---TABLE---
    std::string createDatabase(const std::string& name);
    std::string dropDatabase(const std::string& name);
    std::string useDatabase(const std::string& name);

    // TABLE
    std::string createTable(const std::string& table_name,
        const std::vector<std::string>& columns_name,
        const std::map<std::string, std::string>& colTypes);
    std::string dropTable(const std::string& table_name);
    std::string alterTableAdd(const std::string& table_name, const std::string& col_name, const std::string& col_type);
    std::string alterTableDrop(const std::string& table_name, const std::string& col_name);
    std::string alterTableModify(const std::string& table_name, const std::string& col_name, const std::string& col_type);
private:
    CoreProcess();
};



#endif //COREPROCESS_H

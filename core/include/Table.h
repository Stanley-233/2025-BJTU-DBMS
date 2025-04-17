//
// Created by mzfsh on 25-4-14.
//

#ifndef TABLE_H
#define TABLE_H
#include "csv.hpp"
#include "DataTypes.h"
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

class Table {
    std::string file_name;
    csv::CSVReader *table_reader;
    std::ofstream table_stream;
    std::unordered_map<std::string, TypeHandler> type_getter;
    std::vector<std::string> table_headers;
public:
    explicit Table(const std::string &file_name);
    ~Table();
    static void create_table(std::string,
        const std::string &,
        std::unordered_map<std::string, TypeHandler>,
        const std::vector<std::string> &);
    //void insert_row(const std::vector<BaseType *> &row);
    void drop_table();
    void alter_table_drop_column(const std::string &);
    void alter_table_add_column(const std::string &, int);
    void alter_table_rename_column(const std::string &, const std::string &);
    void alter_table_modify(const std::string &, int);
};



#endif //TABLE_H

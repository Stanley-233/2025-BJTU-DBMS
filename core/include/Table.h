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
    std::unordered_map<std::string, int> type_getter;
    std::vector<std::string> table_headers;
    int check_row(const std::unordered_map<std::string, std::string> &row) const;
    std::vector<std::string> fill_row(std::unordered_map<std::string, std::string> &row);
public:
    explicit Table(const std::string &file_name);
    ~Table();
    static void create_table(std::string,
        const std::string &,
        std::unordered_map<std::string, int>,
        const std::vector<std::string> &);

    void drop_table();
    void alter_table_drop_column(const std::string &);
    void alter_table_add_column(const std::string &, int);
    void alter_table_rename_column(const std::string &, const std::string &);
    void alter_table_modify(const std::string &, int);
    void alter_table_rename_table(const std::string &);
    int insert_row(std::unordered_map<std::string, std::string> &);
    int delete_rows(std::unordered_map<std::string, std::string> &);
    int update_rows(std::unordered_map<std::string, std::string> &,
                    std::unordered_map<std::string, std::string> &);
    std::string select_rows(   std::unordered_map<std::string, std::string> &,
                                            std::vector<std::string> &);
};



#endif //TABLE_H

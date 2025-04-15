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

using namespace std;

class Table {
    csv::CSVReader *table_reader;
    ofstream table_stream;
    unordered_map<string, TypeHandler> type_getter;
    vector<string> table_headers;
public:
    explicit Table(const string &file_name);
    ~Table();
    static Table create_table(string, const string &, unordered_map<string, TypeHandler>, const vector<string> &);
    void insert_row(const vector<string> &row);
};



#endif //TABLE_H

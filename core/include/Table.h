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

using namespace std;
using namespace csv;


class Table {
    CSVReader table_reader;
    ofstream table_stream;
public:
    Table();
    ~Table();
    static Table create_table(string, string, unordered_map<string, BaseType*>);
    void write_row();
};



#endif //TABLE_H

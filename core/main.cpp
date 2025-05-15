#include <iostream>
#include "include/Table.h"

int main() {
    std::string path = "C:/Users/mzfsh/Desktop";
    std::string table_name = "test";
    std::vector<std::string> header = {"Author", "Name", "Email"};
    std::unordered_map<std::string, int> types = {  {"Author", 0},
                                                    {"Name", 1},
                                                    {"Email", 0} };
    Table::create_table(path, table_name, types, header);
    Table t1(path + "/" + table_name);
    std::unordered_map<std::string, std::string> temp_row1{{"Name", "mzf"}, {"Email", "Mzf"}};
    std::unordered_map<std::string, std::string> temp_row2{{"Name", "222"}, {"Email", "111"}};
    t1.insert_row(temp_row1);
    t1.insert_row(temp_row1);
    t1.insert_row(temp_row2);
    t1.insert_row(temp_row2);
    t1.insert_row(temp_row2);
    t1.alter_table_drop_column("Name");
}
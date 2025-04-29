#include <iostream>
#include "include/Table.h"

int main() {
    std::string path = "C:/Users/mzfsh/Desktop";
    std::string table_name = "test";;
    std::vector<std::string> header = {"Author", "Name", "Email"};
    std::unordered_map<std::string, int> types = {    {"Author", 0},
                                                    {"Name", 1},
                                                    {"Email", 0} };
    Table::create_table(path, table_name, types, header);
    Table t(path + "/" + table_name);
    std::unordered_map<std::string, std::string> temp_row1{{"Name", "mzf"}, {"Email", "Mzf"}};
    std::unordered_map<std::string, std::string> temp_row2{{"Name", "222"}, {"Email", "111"}};
    std::cout << t.insert_row(temp_row1);
    std::cout << t.insert_row(temp_row2);
    std::cout << t.insert_row(temp_row2);
    std::cout << t.insert_row(temp_row2);
    std::cout << t.insert_row(temp_row2);
    std::unordered_map<std::string, std::string> temp_condition{{"Email", "111"}};
    std::unordered_map<std::string, std::string> temp_update{{"Email", "222"}};
    std::cout << t.update_rows(temp_condition, temp_update);
}
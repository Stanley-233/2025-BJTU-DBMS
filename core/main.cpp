#include <iostream>
#include "include/Table.h"

int main() {
    std::string path = "C:/Users/mzfsh/Desktop";
    std::string table_name = "test";
    std::vector<std::string> header = {"Author", "Name", "Email"};
    std::vector<std::string> another_header = {"Name", "Phone", "Money"};
    std::unordered_map<std::string, int> types = {  {"Author", 0},
                                                    {"Name", 1},
                                                    {"Email", 0} };
    std::unordered_map<std::string, int> another_types = {{"Phone", 0},
                                                        {"Name", 1},
                                                        {"Money", 0} };
    Table::create_table(path, table_name, types, header);
    Table::create_table(path, table_name + "1", another_types, another_header);
    Table t1(path + "/" + table_name);
    Table t2(path + "/" + table_name + "1");
    std::unordered_map<std::string, std::string> temp_row1{{"Name", "mzf"}, {"Email", "Mzf"}};
    std::unordered_map<std::string, std::string> temp_row2{{"Name", "222"}, {"Email", "111"}};
    t1.insert_row(temp_row1);
    t1.insert_row(temp_row1);
    t1.insert_row(temp_row2);
    t1.insert_row(temp_row2);
    t1.insert_row(temp_row2);
    std::unordered_map<std::string, std::string> temp_row3{{"Name", "mzf"}, {"Phone", "222"}, {"Money", "111"}};
    std::unordered_map<std::string, std::string> temp_condition{{"Email", "Mzf"}};
    t2.insert_row(temp_row3);
    t2.insert_row(temp_row3);
    std::vector<std::string> temp_header = {"Author"};
    std::string temp_on = "Name";
    std::cout << select_with_join(t1, t2, temp_header, temp_condition, temp_on, temp_on);
}
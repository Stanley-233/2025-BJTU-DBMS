#include <iostream>
#include "include/Table.h"

int main() {
    std::string path = "C:/Users/mzfsh/Desktop";
    std::string table_name = "test";;
    std::vector<std::string> header = {"Author", "Name", "Email"};
    std::unordered_map<std::string, TypeHandler> types = {    {"Author", TypeHandler(0)},
                                                    {"Name", TypeHandler(1)},
                                                    {"Email", TypeHandler(0)} };
    Table::create_table(path, table_name, types, header);
    Table t(path + "/" + table_name);
    t.alter_table_rename_table("nooo");
}
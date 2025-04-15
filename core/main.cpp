#include <iostream>
#include "include/Table.h"
using namespace std;

int main() {
    string path = "C:/Users/mzfsh/Desktop";
    string table_name = "test";
    vector<string> header = {"Author", "Name", "Email"};
    unordered_map<string, TypeHandler> types = {    {"Author", TypeHandler(0)},
                                                    {"Name", TypeHandler(0)},
                                                    {"Email", TypeHandler(0)} };

    Table::create_table(path, table_name, types, header);
}
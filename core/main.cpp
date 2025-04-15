#include <iostream>
#include "include/Table.h"
using namespace std;

int main() {
    string path = "C:/Users/mzfsh/Desktop";
    string table_name = "test";
    vector<string> header = {"Author", "Name", "Email"};
    unordered_map<string, TypeHandler> types = {    {"Author", TypeHandler(Types::TEXT)},
                                                    {"Name", TypeHandler(Types::TEXT)},
                                                    {"Email", TypeHandler(Types::TEXT)} };

    Table::create_table(path, table_name, types, header);
}
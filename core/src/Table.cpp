//
// Created by mzfsh on 25-4-14.
//

#include "Table.h"
#include <filesystem>

Table::Table(const string &file_name) {
    this->file_name = file_name + ".csv";
    CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .no_header();
    CSVReader header_reader(file_name + ".header", table_format);
    CSVRow header, type;
    header_reader.read_row(header);
    header_reader.read_row(type);
    for (auto && i : header) {
        table_headers.emplace_back(i.get<string>());
    }
    int index = 0;
    for (auto && i : type) {
        type_getter.emplace(table_headers[index++],TypeHandler(i.get<int>()));
    }
    table_format.header_row(0);
    table_reader = new CSVReader(this->file_name, table_format);
}

Table::~Table() {
    delete table_reader;
}

Table* Table::create_table(  string path,
                            const string &table_name,
                            unordered_map<string, TypeHandler> h_types,
                            const vector<string> &t_headers    ) {
    //制造合法路径
    if (path[path.size() - 1] != '/')
        path += '/';
    path += table_name;
    //文件数据格式写入
    ofstream temp_writer(path + ".header", ios::trunc);
    auto header_creator = make_csv_writer(temp_writer);
    set_decimal_places(6);
    header_creator << t_headers;
    vector<string> t_types;
    for (const auto & s : t_headers)
        t_types.emplace_back(to_string(h_types[s].get_type_id()));
    header_creator << t_types;
    temp_writer.close();
    header_creator.flush();
    //在表中写入表头
    ofstream csv_header_writer(path+".csv", ios::trunc);
    auto csv_header_creator = make_csv_writer(csv_header_writer);
    csv_header_creator << t_headers;
    return new Table(path);
}

void Table::insert_row(const vector<string> &row) {
    auto table_writer = make_csv_writer(table_stream);
    table_writer << row;
}

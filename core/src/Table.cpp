//
// Created by mzfsh on 25-4-14.
//

#include "Table.h"

Table::Table(const string &file_name) {
    csv::CSVFormat table_format;
    table_format.delimiter('\t')
                .quote(false)
                .no_header();
    table_stream.open(file_name);
    csv::CSVReader header_reader(file_name + ".header", table_format);
    csv::CSVRow header, type;
    header_reader.read_row(header);
    header_reader.read_row(type);
    for (int i = 0; i < header.size(); i++) {
        auto column_name = header[i].get<string>();
        type_getter.emplace(column_name, header[i].get<int>());
        table_headers.emplace_back(column_name);
    }
    table_format.column_names(table_headers);
    table_reader = new csv::CSVReader(file_name, table_format);
}

Table::~Table() {
    delete table_reader;
    table_stream.close();
}

Table Table::create_table(  string path,
                            const string &table_name,
                            unordered_map<string, TypeHandler> h_types,
                            const vector<string> &t_headers    ) {
    if (path[path.size() - 1] != '/')
        path += '/';
    path += table_name + ".csv";
    ofstream temp_header_writer(path + ".header", ios::trunc);
    auto header_creator = csv::make_csv_writer(temp_header_writer);
    csv::set_decimal_places(6);
    header_creator << t_headers;
    vector<string> t_types;
    for (const auto & s : t_headers)
        t_types.emplace_back(to_string(h_types[s].get_type_id()));
    header_creator << t_types;
    temp_header_writer.close();
    return Table(path);
}

void Table::insert_row(const vector<string> &row) {
    auto table_writer = csv::make_csv_writer(table_stream);
    table_writer << row;
}

//
// Created by mzfsh on 25-4-14.
//

#include "Table.h"
#include <filesystem>

Table::Table(const std::string &file_name) {
    this->file_name = file_name + ".csv";
    csv::CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .no_header();
    csv::CSVReader header_reader(file_name + ".header", table_format);
    csv::CSVRow header, type;
    header_reader.read_row(header);
    header_reader.read_row(type);
    for (auto && i : header) {
        table_headers.emplace_back(i.get<std::string>());
    }
    int index = 0;
    for (auto && i : type) {
        type_getter.emplace(table_headers[index++],TypeHandler(i.get<int>()));
    }
    table_format.header_row(0);
    table_reader = new csv::CSVReader(this->file_name, table_format);
}

Table::~Table() {
    delete table_reader;
}

void Table::create_table(  std::string path,
                            const std::string &table_name,
                            std::unordered_map<std::string, TypeHandler> h_types,
                            const std::vector<std::string> &t_headers    ) {
    //制造合法路径
    if (path[path.size() - 1] != '/')
        path += '/';
    path += table_name;
    //文件数据格式写入
    std::ofstream temp_writer(path + ".header", std::ios::trunc);
    auto header_creator = csv::make_csv_writer(temp_writer);
    csv::set_decimal_places(6);
    header_creator << t_headers;
    std::vector<std::string> t_types;
    for (const auto & s : t_headers)
        t_types.emplace_back(std::to_string(h_types[s].get_type_id()));
    header_creator << t_types;
    temp_writer.close();
    header_creator.flush();
    //在表中写入表头
    std::ofstream csv_header_writer(path+".csv", std::ios::trunc);
    auto csv_header_creator = csv::make_csv_writer(csv_header_writer);
    csv_header_creator << t_headers;
}

void Table::insert_row(const std::vector<std::string> &row) {
    auto table_writer = csv::make_csv_writer(table_stream);
    table_writer << row;
}

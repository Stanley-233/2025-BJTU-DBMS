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
    if (!table_reader)
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

/*void Table::insert_row(const std::vector<BaseType*> &row) {
    std::vector<std::string> output_row(row.size());
    for (auto & i : row)
        output_row.emplace_back(i->to_string());
    std::ofstream table(this->file_name, std::ios::trunc);
    auto table_writer = csv::make_csv_writer(table);
    table_writer << output_row;
}*/

void Table::drop_table(){
    delete table_reader;
    table_reader = nullptr;
    std::filesystem::path table_path(this->file_name), temp_path = table_path;
    std::filesystem::rename(table_path, temp_path.replace_extension(std::filesystem::path(".del")));
    table_path.replace_extension(".header");
    std::filesystem::rename(table_path, temp_path.replace_extension(std::filesystem::path(".hdl")));
}

void Table::alter_table_drop_column(const std::string & column_name) {
    for (int i= 0; i < table_headers.size(); ++i)
        if (table_headers[i] == column_name)
            table_headers.erase(table_headers.begin() + i);
    type_getter.erase(column_name);
    std::filesystem::path table_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    std::ofstream header_stream(table_path, std::ios::trunc);
    auto header_writer = csv::make_csv_writer(header_stream);
    header_writer << table_headers;
    std::vector<int> column_types(0);
    for (auto && i : table_headers)
        column_types.emplace_back(type_getter[i].get_type_id());
    header_writer << column_types;
    header_stream.close();
}

void Table::alter_table_add_column(const std::string & column_name, int type) {
    table_headers.emplace_back(column_name);
    type_getter.emplace(column_name, type);
    std::filesystem::path table_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    std::ofstream header_stream(table_path, std::ios::trunc);
    auto header_writer = csv::make_csv_writer(header_stream);
    header_writer << table_headers;
    std::vector<int> column_types(0);
    for (auto && i : table_headers)
        column_types.emplace_back(type_getter[i].get_type_id());
    header_writer << column_types;
    header_stream.close();
}

void Table::alter_table_rename_column(const std::string &oldname, const std::string &newname) {
    for (auto & i : table_headers)
        if (i == oldname)
            i = newname;
    int temp = type_getter[oldname].get_type_id();
    type_getter.erase(oldname);
    type_getter.emplace(newname, temp);
    std::filesystem::path table_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    std::ofstream header_stream(table_path, std::ios::trunc);
    auto header_writer = csv::make_csv_writer(header_stream);
    header_writer << table_headers;
    std::vector<int> column_types(0);
    for (auto && i : table_headers)
        column_types.emplace_back(type_getter[i].get_type_id());
    header_writer << column_types;
    header_stream.close();
}

void Table::alter_table_modify(const std::string &column_name, int type) {
    type_getter[column_name] = TypeHandler(type);
    std::filesystem::path table_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    std::ofstream header_stream(table_path, std::ios::trunc);
    auto header_writer = csv::make_csv_writer(header_stream);
    header_writer << table_headers;
    std::vector<int> column_types(0);
    for (auto && i : table_headers)
        column_types.emplace_back(type_getter[i].get_type_id());
    header_writer << column_types;
    header_stream.close();
}

void Table::alter_table_rename_table(const std::string & new_name) {
    delete table_reader;
    std::filesystem::path table_path(this->file_name), new_path = table_path;
    new_path.replace_filename(std::filesystem::path(new_name + ".csv"));
    std::filesystem::rename(table_path, new_path);
    table_path.assign(file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    new_path.replace_extension(std::filesystem::path(".header"));
    std::filesystem::rename(table_path, new_path);
    file_name = new_path.replace_extension(std::filesystem::path(".csv")).string();
    csv::CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .header_row(0);
    table_reader = new csv::CSVReader(file_name, table_format);
}


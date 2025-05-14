//
// Created by mzfsh on 25-4-14.
//

#include "Table.h"
#include <filesystem>
#include <algorithm>

int Table::check_row(const std::unordered_map<std::string, std::string> &row) const {
    //错误码：
    //1: too many values.
    //2: certain column don't exist.
    if (row.size() > table_headers.size())
        return 1;
    for (const auto &p :  row) {
        if (std::find(table_headers.begin(), table_headers.end(), p.first) == table_headers.end())
            return 2;
    }
    return 0;
}

std::vector<std::string> Table::fill_row(std::unordered_map<std::string, std::string> &row) {
    std::vector<std::string> result(0);
    for (const auto & h : table_headers) {
        if (row.count(h) == 0) {
            result.emplace_back("%NULL%");
            continue;
        }
        result.emplace_back(row[h]);
    }
    return result;
}

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
        type_getter.emplace(table_headers[index++],i.get<int>());
    }
    table_format.header_row(0);
    table_reader = new csv::CSVReader(this->file_name, table_format);
}

Table::~Table() {
    delete table_reader;
    table_stream.close();
}

void Table::create_table(  std::string path,
                            const std::string &table_name,
                            std::unordered_map<std::string, int> h_types,
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
        t_types.emplace_back(std::to_string(h_types[s]));
    header_creator << t_types;
    temp_writer.close();
    header_creator.flush();
    //在表中写入表头
    std::ofstream csv_header_writer(path+".csv", std::ios::trunc);
    auto csv_header_creator = csv::make_csv_writer(csv_header_writer);
    csv_header_creator << t_headers;
}

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
        column_types.emplace_back(type_getter[i]);
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
        column_types.emplace_back(type_getter[i]);
    header_writer << column_types;
    header_stream.close();
}

void Table::alter_table_rename_column(const std::string &oldname, const std::string &newname) {
    for (auto & i : table_headers)
        if (i == oldname)
            i = newname;
    int temp = type_getter[oldname];
    type_getter.erase(oldname);
    type_getter.emplace(newname, temp);
    std::filesystem::path table_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    std::ofstream header_stream(table_path, std::ios::trunc);
    auto header_writer = csv::make_csv_writer(header_stream);
    header_writer << table_headers;
    std::vector<int> column_types(0);
    for (auto && i : table_headers)
        column_types.emplace_back(type_getter[i]);
    header_writer << column_types;
    header_stream.close();
}

void Table::alter_table_modify(const std::string &column_name, const int type) {
    type_getter[column_name] = type;
    std::filesystem::path table_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".header"));
    std::ofstream header_stream(table_path, std::ios::trunc);
    auto header_writer = csv::make_csv_writer(header_stream);
    header_writer << table_headers;
    std::vector<int> column_types(0);
    for (auto && i : table_headers)
        column_types.emplace_back(type_getter[i]);
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

int Table::insert_row(std::unordered_map<std::string, std::string>& row) {
    const int error_code = check_row(row);
    if (error_code != 0)
        return error_code;
    std::ofstream temp_writer(file_name, std::ios::app);
    auto row_writer = csv::make_csv_writer(temp_writer);
    row_writer << fill_row(row);
    temp_writer.close();
    return 0;
}

int Table::delete_rows(std::unordered_map<std::string, std::string> & conditions) {
    if (const int error_code = check_row(conditions); error_code != 0)
        return error_code;
    int rows_deleted = 0;
    std::filesystem::path table_path(this->file_name), original_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".tmp"));
    std::ofstream temp_writer(table_path, std::ios::trunc);
    auto row_writer = csv::make_csv_writer(temp_writer);
    row_writer << table_headers;
    for (auto & row : *table_reader) {
        bool not_deleted = false;
        for (auto & c : conditions)
            if (row[c.first].get<std::string>() != c.second) {
                not_deleted = true;
            }
        if (not_deleted) {
            std::vector<std::string> temp_row(0);
            for (auto & h : table_headers)
                temp_row.emplace_back(row[h].get<std::string>());
            row_writer << temp_row;
            continue;
        }
        ++rows_deleted;
    }
    temp_writer.close();
    delete table_reader;
    table_path.replace_extension(std::filesystem::path(".del"));
    std::filesystem::rename(original_path, table_path);
    original_path.replace_extension(std::filesystem::path(".tmp"));
    table_path.replace_extension(std::filesystem::path(".csv"));
    std::filesystem::rename(original_path, table_path);
    csv::CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .header_row(0);
    table_reader = new csv::CSVReader(file_name, table_format);
    return rows_deleted;
}

int Table::update_rows( std::unordered_map<std::string, std::string> &conditions,
                        std::unordered_map<std::string, std::string> &values_to_be_updated) {
    if (const int error_code = check_row(conditions); error_code != 0)
        return error_code;
    if (const int error_code = check_row(values_to_be_updated); error_code != 0)
        return error_code;
    int rows_updated = 0;
    std::filesystem::path table_path(this->file_name), original_path(this->file_name);
    table_path.replace_extension(std::filesystem::path(".tmp"));
    std::ofstream temp_writer(table_path, std::ios::trunc);
    auto row_writer = csv::make_csv_writer(temp_writer);
    row_writer << table_headers;
    for (auto & row : *table_reader) {
        bool not_updated = false;
        for (auto & c : conditions)
            if (row[c.first].get<std::string>() != c.second)
                not_updated = true;
        if (not_updated) {
            std::vector<std::string> temp_row(0);
            for (auto & h : table_headers)
                temp_row.emplace_back(row[h].get<std::string>());
            row_writer << temp_row;
        }
        else {
            ++rows_updated;
            std::vector<std::string> temp_row(0);
            for (auto & h : table_headers) {
                if (values_to_be_updated.find(h) != values_to_be_updated.end())
                    temp_row.emplace_back(values_to_be_updated[h]);
                else
                    temp_row.emplace_back(row[h].get<std::string>());
            }
            row_writer << temp_row;
        }
    }
    temp_writer.close();
    delete table_reader;
    table_path.replace_extension(std::filesystem::path(".del"));
    std::filesystem::rename(original_path, table_path);
    original_path.replace_extension(std::filesystem::path(".tmp"));
    table_path.replace_extension(std::filesystem::path(".csv"));
    std::filesystem::rename(original_path, table_path);
    csv::CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .header_row(0);
    table_reader = new csv::CSVReader(file_name, table_format);
    return rows_updated;
}

std::string Table::select_rows(std::unordered_map<std::string, std::string> &conditions,
                                            std::vector<std::string> &selected_columns) {
    if (selected_columns.empty())
        selected_columns = table_headers;
    else if (selected_columns.size() > table_headers.size()) {
        return "Too Many Columns!";
    }
    for (auto & c : selected_columns)
        if (std::find(table_headers.begin(), table_headers.end(), c) == table_headers.end())
            return "Invalid Column!";
    switch (check_row(conditions)) {
        case 0:
            break;
        case 1:
            return "Too Many Columns!";
        case 2:
            return "Invalid Columns!";
        default:
            return "Super Big Mistakes!";
    }
    std::stringstream result;
    auto output_table_writer = csv::make_csv_writer(result);
    output_table_writer << selected_columns;
    for (auto & row : *table_reader) {
        bool not_selected = false;
        for (auto & c : conditions)
            if (row[c.first].get<std::string>() != c.second)
                not_selected = true;
        if (!not_selected) {
            std::vector<std::string> temp_row(0);
            for (auto & h : selected_columns)
                temp_row.emplace_back(row[h].get<std::string>());
            output_table_writer << temp_row;
        }
    }
    delete table_reader;
    csv::CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .header_row(0);
    table_reader = new csv::CSVReader(file_name, table_format);
    return result.str();
}

std::string select_with_join(
    Table &table1, Table &table2,
    std::vector<std::string> &selected_columns,
    std::unordered_map<std::string, std::string> &conditions,
    std::string &on_table1_column, std::string &on_table2_column) {
    std::vector<std::string> table1_columns(0);
    std::vector<std::string> table2_columns(0);
    std::unordered_map<std::string, std::string> table1_conditions(0);
    std::unordered_map<std::string, std::string> table2_conditions(0);
    for (auto & c : selected_columns) {
        if (std::find(table1.table_headers.begin(), table1.table_headers.end(), c) != table1.table_headers.end()) {
            table1_columns.emplace_back(c);
            continue;
        }
        if (std::find(table2.table_headers.begin(), table2.table_headers.end(), c) != table2.table_headers.end()) {
            table2_columns.emplace_back(c);
            continue;
        }
        return "Invalid Column!";
    }
    for (auto & c : conditions) {
        if (std::find(table1.table_headers.begin(), table1.table_headers.end(), c.first) != table1.table_headers.end()) {
            table1_conditions.emplace(c.first, c.second);
            continue;
        }
        if (std::find(table2.table_headers.begin(), table2.table_headers.end(), c.first) != table2.table_headers.end()) {
            table2_conditions.emplace(c.first, c.second);
            continue;
        }
        return "Invalid Column!";
    }
    std::stringstream result;
    auto output_table_writer = csv::make_csv_writer(result);
    output_table_writer << selected_columns;
    for (auto & row1 : *table1.table_reader) {
        for (auto & row2 : *table2.table_reader) {
            bool not_selected = false;
            for (auto & c : table1_conditions)
                if (row1[c.first].get<std::string>() != c.second)
                    not_selected = true;
            for (auto & c : table2_conditions)
                if (row2[c.first].get<std::string>() != c.second)
                    not_selected = true;
            if (!not_selected) {
                if (row1[on_table1_column].get<std::string>() == row2[on_table2_column].get<std::string>()) {
                    std::vector<std::string> temp_row(0);
                    for (auto & h : selected_columns) {
                        if (std::find(table1_columns.begin(), table1_columns.end(), h) != table1_columns.end())
                            temp_row.emplace_back(row1[h].get<std::string>());
                        else
                            temp_row.emplace_back(row2[h].get<std::string>());
                    }
                    output_table_writer << temp_row;
                }
            }
        }
        delete table2.table_reader;
        csv::CSVFormat table_format;
        table_format.delimiter(',')
                    .quote('"')
                    .header_row(0);
        table2.table_reader = new csv::CSVReader(table2.file_name, table_format);
    }
    delete table2.table_reader;
    csv::CSVFormat table_format;
    table_format.delimiter(',')
                .quote('"')
                .header_row(0);
    table2.table_reader = new csv::CSVReader(table2.file_name, table_format);
    delete table1.table_reader;
    table2.table_reader = new csv::CSVReader(table2.file_name, table_format);
    return result.str();
}














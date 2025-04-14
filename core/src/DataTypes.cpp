//
// Created by mzfsh on 25-4-14.
//

#include "../include/DataTypes.h"

#include <algorithm>

void INTEGER::get_value_from_string(const string & str) {
    memcpy(&value, str.c_str(), sizeof(value));
}

INTEGER::operator string() const {
    string result;
    memcpy(result.data(), &value, sizeof(value));
    result.resize(sizeof(value));
    return result;
}

INTEGER::INTEGER(const string &v) {
    get_value_from_string(v);
}

TEXT::operator string() const {
    return text;
}

void TEXT::get_value_from_string(const string & str) {
    text = str;
}

bool DECIMAL::is_invalid() {
    if (integer.size() > length_of_integer || decimal.size() > length_of_decimal)
        return false;
    if (all_of(integer.begin(), integer.end(), ::isdigit))
        return false;
    if (all_of(decimal.begin(), decimal.end(), ::isdigit))
        return false;
    return true;
}

DECIMAL::operator string() const {
    return integer + "." + decimal;
}

void DECIMAL::get_value_from_string(const string & str) {
    const auto i = str.find_first_of('.');
    integer = str.substr(0, i);
    decimal = str.substr(i + 1);
}

DECIMAL::DECIMAL(const string &v) {
    get_value_from_string(v);
}

int TypeHandler::get_type_id() const {
    return static_cast<int>(type);
}

BaseType * TypeHandler::create_unit(const string & value) const{
    switch (type) {
        case Types::TEXT:
            return new TEXT(value);
        case Types::INTEGER:
            return new INTEGER(value);
        case Types::DECIMAL:
            return new DECIMAL(value);
    }
}

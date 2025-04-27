//
// Created by mzfsh on 25-4-14.
//

#include "../include/DataTypes.h"

#include <algorithm>
#include <csv.hpp>

void INTEGER::set_value_from_string(const std::string & str) {
    value = std::stoi(str);
}

INTEGER::operator std::string() const {
    return std::to_string(value);
}

void MYTEXT::set_value_from_string(const std::string & str) {
    text = str;
}

MYTEXT::operator std::string() const {
    return text;
}

bool POINTING::is_invalid() {
    if (all_of(integer.begin(), integer.end(), isdigit))
        return false;
    if (all_of(decimal.begin(), decimal.end(), isdigit))
        return false;
    return true;
}

POINTING::operator std::string() const {
    return integer + "." + decimal;
}

void POINTING::set_value_from_string(const std::string & str) {
    const auto i = str.find_first_of('.');
    integer = str.substr(0, i);
    decimal = str.substr(i + 1);
}

POINTING::POINTING(const std::string &v) {
    set_value_from_string(v);
}

BaseType * TypeHandler::create_unit(const int type, const std::string & value){
    switch (type) {
        case 0:
            return new MYTEXT(value);
        case 1:
            return new INTEGER(value);
        case 2:
            return new POINTING(value);
        default:
            return nullptr;
    }
}

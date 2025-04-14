//
// Created by mzfsh on 25-4-14.
//

#ifndef DATATYPE_H
#define DATATYPE_H
#include <string>
#include <utility>
using namespace std;

class BaseType {
public:
    virtual ~BaseType() = default;
    virtual void get_value_from_string(const string&) = 0;
    explicit virtual operator string() const = 0;
};

class INTEGER final : public BaseType{
    int value = 0;
public:
    INTEGER() = default;
    explicit INTEGER(const int v) : value(v) {}
    explicit operator int() const {return value;}
    [[nodiscard]] int getValue() const {return value;}
    void get_value_from_string(const string&) override;
    explicit operator string() const override;
};

class TEXT final : public BaseType {
    string text;
public:
    explicit TEXT(string v) : text(std::move(v)) {}
    explicit operator string() const override;
    void get_value_from_string(const string&) override;
};

class DECIMAL final : public BaseType {
    string integer;
    int length_of_integer;
    string decimal;
    int length_of_decimal;
public:
    explicit DECIMAL(const int all, const int d) : length_of_integer(all-d), length_of_decimal(d) {}
    bool is_invalid();
    explicit operator string() const override;
    void get_value_from_string(const string&) override;
};

enum class Types : unsigned int {
    INTEGER,
    TEXT,
    DECIMAL,
};

class TypeHandler {
    Types type;
public:
    explicit TypeHandler(const Types t) : type(t) {}
    BaseType * create_unit();
};

#endif //DATATYPE_H

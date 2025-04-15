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
    BaseType() = default;
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
    explicit INTEGER(const string & v);
};

class MYTEXT final : public BaseType {
    std::string text;
public:
    explicit MYTEXT(std::string v) : text(std::move(v)) {}
    ~MYTEXT() override = default;
    explicit operator string() const override;
    void get_value_from_string(const string&) override;
};

class POINTING final : public BaseType {
    string integer;
    int length_of_integer;
    string decimal;
    int length_of_decimal;
public:
    explicit POINTING(const int all, const int d) : length_of_integer(all-d), length_of_decimal(d) {}
    bool is_invalid();
    explicit operator string() const override;
    void get_value_from_string(const string&) override;
    explicit POINTING(const string & v);
};

class TypeHandler {
    // 0 - TEXT
    // 1 - 整数
    // 2 - 小数
    int type;
public:
    explicit TypeHandler(const int t) : type(t) {}
    [[nodiscard]] int get_type_id() const;
    BaseType * create_unit(const string&) const;
};

#endif //DATATYPE_H

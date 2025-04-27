//
// Created by mzfsh on 25-4-14.
//

#ifndef DATATYPE_H
#define DATATYPE_H
#include <cstring>
#include <string>
#include <utility>

class BaseType {
public:
    BaseType() = default;
    virtual ~BaseType() = default;
    virtual void set_value_from_string(const std::string&) = 0;
    explicit virtual operator std::string() const = 0;
};

class INTEGER final : public BaseType{
    int value = 0;
public:
    INTEGER() = default;
    explicit operator int() const {return value;}
    [[nodiscard]] int getValue() const {return value;}
    void set_value_from_string(const std::string&) override;
    explicit operator std::string() const override;
    explicit INTEGER(const std::string & v) : value(std::stoi(v)) {}
};

class MYTEXT final : public BaseType {
    std::string text;
public:
    explicit MYTEXT(std::string v) : text(std::move(v)) {}
    ~MYTEXT() override = default;
    explicit operator std::string() const override;
    void set_value_from_string(const std::string&) override;
};

class POINTING final : public BaseType {
    std::string integer;
    std::string decimal;
public:
    bool is_invalid();
    explicit operator std::string() const override;
    void set_value_from_string(const std::string&) override;
    explicit POINTING(const std::string & v);
};

class TypeHandler {
    // 0 - TEXT
    // 1 - 整数
    // 2 - 小数
public:
    explicit TypeHandler() = default;
    [[nodiscard]] static BaseType * create_unit(int, const std::string&);
};

#endif //DATATYPE_H

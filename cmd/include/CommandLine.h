//
// Created by stanl on 25-4-14.
//

#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <string>
#include <vector>

#include "Parser.h"

class CommandLine {
public:
    // 获取单例对象
    static CommandLine& getInstance();
    int processSQL(const std::string& input, std::vector<std::string>& tokens);
    // 开始运行
    void start();
    Parser& getParser() { return _parser; }
private:
    CommandLine() {}

    // Token序列
    std::vector<std::string> seq;

    // Parser
    Parser _parser;

    std::string _username;

    // 预处理(trim)
    static inline int preProcess(std::string& str);

    // Running
    static inline int tokenize(std::string& str, std::vector<std::string>& result);

    // 批处理
    inline int ReadSqlBatch(std::string& file_name);
};   



#endif //COMMANDLINE_H

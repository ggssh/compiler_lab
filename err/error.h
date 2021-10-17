//
// Created by ggssh on 2021/10/17 0017.
//

#ifndef COMPILER_LAB_ERROR_H
#define COMPILER_LAB_ERROR_H

#include "file_reader.h"
using namespace std;
/**
 * 对分析过程中的错误进行处理
 */
class ErrorLogger {
private:
    std::ostream &err;

    void report(size_t line, size_t col, const std::string &msg, const std::string &prefix);

public:
    explicit ErrorLogger(std::ostream &error_stream);

    /**
     * 打印错误
     * @param lexer
     * @param msg
     */
    void LogError(const FileReader *reader, const std::string &msg);

    /**
     * 打印警告
     * @param lexer
     * @param msg
     */
    void LogWarning(const FileReader *reader, const std::string &msg);
};


#endif //COMPILER_LAB_ERROR_H

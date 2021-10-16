//
// Created by ggssh on 2021/10/16 0016.
//
#include <iostream>

#ifndef COMPILER_LAB_FILE_READER_H
#define COMPILER_LAB_FILE_READER_H

class FileReader{
    std::string content;// 文件内容
    unsigned int cur_index = 0; // 当前处理到的位置
    unsigned int content_length = 0; //文件内容的长度
    unsigned int lineno = 0;// 行号
    unsigned int colno = 0;// 列号

public:
    explicit FileReader(const std::string &path);

    unsigned int getLineno() const;

    unsigned int getColno() const;

    int get_char();// 获取当前字符

    int seek();// 往后看一个字符
};
#endif //COMPILER_LAB_FILE_READER_H

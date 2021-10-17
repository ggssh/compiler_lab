//
// Created by ggssh on 2021/10/16 0016.
//
#include <iostream>

#ifndef COMPILER_LAB_FILE_READER_H
#define COMPILER_LAB_FILE_READER_H

class FileReader {
    std::string content;// 文件内容
    unsigned int cur_index = 0; // 当前处理到的位置
    unsigned int content_length = 0; //文件内容的长度
    unsigned int lineno = 0;// 行号
    unsigned int colno = 0;// 列号

public:
    /**
     *
     * @param path
     */
    explicit FileReader(const std::string &path);

    /**
     * 获得行号
     * @return
     */
    unsigned int getLineno() const;

    /**
     * 获得列号
     * @return
     */
    unsigned int getColno() const;

    /**
     * 获取当前index的字符并使index+1
     * @return
     */
    int get_char();// 获取当前字符

    /**
     * 查看当前index的字符
     * @return
     */
    int seek();// 往后看一个字符
};

#endif //COMPILER_LAB_FILE_READER_H

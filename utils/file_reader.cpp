//
// Created by ggssh on 2021/10/16 0016.
//

#include <fstream>
#include <sstream>
#include "file_reader.h"

unsigned int FileReader::getLineno() const {
    return lineno;
}

unsigned int FileReader::getColno() const {
    return colno;
}

FileReader::FileReader(const std::string &path) {
    std::ifstream ifstream(path);
    if (ifstream.is_open()){
        std::stringstream buffer;
        buffer<<ifstream.rdbuf();
        ifstream.close();
        content = std::string (buffer.str());
        content_length = content.size();
    } else{
        fprintf(stderr,"Can't open file");
        exit(-1);
    }
}

int FileReader::get_char() {
    if (cur_index<content_length){
        char c = content[cur_index++];
        if (c=='\n'){
            lineno++;
            colno=0;
        }
        return c;
    }
    return EOF;
}

int FileReader::seek() {
    if (cur_index<content_length)
        return content[cur_index];
    return EOF;
}

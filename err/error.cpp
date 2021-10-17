//
// Created by ggssh on 2021/10/17 0017.
//

#include "error.h"

void ErrorLogger::report(size_t line, size_t col, const string &msg, const string &prefix) {
    err << "[" << prefix << "]" << "at position " << line << ":" << col << " " << msg << std::endl;
}

ErrorLogger::ErrorLogger(ostream &error_stream) : err(error_stream) {
}

void ErrorLogger::LogError(const FileReader *reader, const std::string &msg) {
    report(reader->getLineno(), reader->getColno(), msg, "ERROR");
}

void ErrorLogger::LogWarning(const FileReader *reader, const std::string &msg) {
    report(reader->getLineno(), reader->getColno(), msg, "WARNING");
}

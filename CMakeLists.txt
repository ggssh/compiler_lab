cmake_minimum_required(VERSION 3.16)
project(compiler_lab)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(lexer LEXER)
aux_source_directory(. ROOT)
aux_source_directory(utils UTILS)

include_directories(compiler_lab lexer utils)
#add_executable(compiler_lab main.cpp lexer/token.cpp lexer/token.h lexer/lexer.cpp lexer/lexer.h utils/file_reader.cpp utils/file_reader.h)
add_executable(compiler_lab
        main.cpp
        ${LEXER}
        ${UTILS})
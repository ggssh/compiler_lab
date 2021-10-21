cmake_minimum_required(VERSION 3.16)
project(compiler_lab)

# 设置为C++11标准
set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. ROOT)
message(STATUS "root根目录:${ROOT}")
aux_source_directory(lexer LEXER)
message(STATUS "lexer目录:${LEXER}")
aux_source_directory(utils UTILS)
message(STATUS "utils目录:${UTILS}")
aux_source_directory(err ERR)
message(STATUS "err目录:${ERR}")
aux_source_directory(parser PARSER)
message(STATUS "parser目录:${PARSER}")

# 如果READ_FROM_ARGV则从命令行参数中读取文件
add_compile_definitions(compiler_lab PUBLIC RAED_FROM_ARGV)
# 如果DEBUG_FLAG开启则在控制台输出
add_compile_definitions(compiler_lab PUBLIC DEBUG_FLAG)
# 如果TIMING开启则输出运行的时间
add_compile_definitions(compiler_lab PUBLIC TIMING)

include_directories(compiler_lab lexer utils err parser)
add_executable(compiler_lab
        main.cpp
        ${LEXER}
        ${UTILS}
        ${ERR}
        ${PARSER})
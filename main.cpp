#include <iostream>
#include <fstream>
#include "file_reader.h"
#include "lexer.h"
#include "parser.h"

#ifdef TIMING

#include <ctime>

#endif

int main(int argc, const char *argv[]) {
#ifdef TIMING
    // 开启计时
    double dur;
    time_t start, end;
    start = clock();
#endif

#ifdef RAED_FROM_ARGV
    FileReader fileReader(argv[1]);
#else
    FileReader fileReader("testfile.txt");
#endif
    Lexer lexer(&fileReader, cerr);
    std::ofstream out("output.txt");
//    while (1) {
//        Token token = lexer.get_token();
//        if (token.type == TokenType::EOFTOK)
//            break;
//#ifdef DEBUG_FLAG
//        std::cout << lexer.map.at(token.type) << ":" << token.literal << std::endl;
//#endif
//        out << lexer.map.at(token.type) << " " << token.literal << std::endl;
//    }
    Parser parser(lexer, out);
    parser.parseProgram();
#ifdef TIMING
    end = clock();
    dur = (double) (end - start);
    printf("Use Time:%f s\n", (dur / CLOCKS_PER_SEC));
#endif

    return 0;
}

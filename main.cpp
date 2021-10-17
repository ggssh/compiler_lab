#include <iostream>
#include <fstream>
#include "file_reader.h"
#include "lexer.h"

#ifdef TIMING
#include <time.h>
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
    Lexer lexer(&fileReader);
    std::ofstream out("output.txt");
    while (1) {
        Token token = lexer.get_token();
        if (token.type == TokenType::EOFTOK)
            break;
#ifdef DEBUG_FLAG
//        std::cout << lexer.map.at(token.type) << ":" << token.literal << std::endl;
#endif
//        out << lexer.map.at(token.type) << " " << token.literal << std::endl;
        out << token.get_type() << " " << token.literal << std::endl;
    }
#ifdef TIMING
    end = clock();
    dur = (double) (end - start);
    printf("Use Time:%f s\n", (dur / CLOCKS_PER_SEC));
#endif
    return 0;
}

#include <iostream>
#include <fstream>

#include "file_reader.h"
#include "lexer.h"

int main(int argc, const char *argv[]) {
//    std::ifstream ifstream;
//    ifstream.open(argv[1]);
    FileReader fileReader(argv[1]);
//    FileReader fileReader("testfile.txt");
    Lexer lexer(&fileReader);
    std::ofstream out("output.txt");
    while (1) {
        Token token = lexer.get_token();
        if (token.type == TokenType::EOFTOK)
            break;
//        std::cout<<token.type<<":"<<token.literal<<std::endl;
        out << lexer.map.at(token.type) << " " << token.literal << std::endl;
    }
//    std::cout << "Hello, World!" << std::endl;
    return 0;
}

#include <iostream> //To work with input and output
#include <fstream>  //To work with files
#include <string>   //To work with strings
#include "headers/helpers.h"
#include "headers/parser.h"
#include "headers/code_writer.h"

int main(int argc, char *argv[])
{
    Parser parser = Parser(argc, argv);
    std::string inputfileName = argv[1];
    std::string fileName = inputfileName.substr(0, inputfileName.length() - 3);
    CodeWriter codeWriter = CodeWriter(fileName);
    while (parser.hasMoreCommands())
    {
        parser.advance();
        if (!parser.isWhiteSpaceOrComment())
        {
            if (parser.commandType() == C_ARITHMETIC)
            {
                codeWriter.writeArithmetic(parser.currentCommand);
            }
            else if (parser.commandType() == C_PUSH || parser.commandType() == C_POP)
            {
                codeWriter.writePushPop(parser.commandType(), parser.arg1(), std::stoi(parser.arg2()));
            }
            std::cout << parser.currentCommand << std::endl;
        }
    }
    return 0;
}

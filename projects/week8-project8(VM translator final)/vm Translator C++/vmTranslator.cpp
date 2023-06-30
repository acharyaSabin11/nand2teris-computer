#include <iostream> //To work with input and output
#include <fstream>  //To work with files
#include <string>   //To work with strings
#include "headers/helpers.h"
#include "headers/parser.h"
#include "headers/code_writer.h"
#include <filesystem>

// namespaces definiton
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    ExtendedList fileNameList;
    bool isFile = true;
    std::ifstream inputFile;
    char *inputName = argv[1];
    if (argc == 2)
    {
        // trying to open the file considering it to be a folder. If it is not a folder exception is thrown. which is handled and then it is checked for to ba vm file.
        try
        {

            for (const auto &entry : fs::directory_iterator(inputName))
            {
                ExtendedString fileName = entry.path().u8string();
                if (fileName.substr(fileName.length() - 3, fileName.length()) == ".vm")
                {
                    fileNameList.add(entry.path().u8string());
                }
            }
            isFile = false;
        }
        catch (...)
        {
            std::string inputfileName = inputName;
            inputFile.open(inputfileName);
            if (!inputFile)
            {
                std::cout << "Provided file doesn't exist";
                exit(0);
            }
            if (inputfileName.substr(inputfileName.length() - 3, inputfileName.length()) != ".vm")
            {
                std::cout << "The provided file is not of extension '.vm'";
                exit(0);
            }
            else
            {
                fileNameList.add(inputfileName);
            }
        }
    }
    else
    {
        std::cout << "One positional argument (file name) is expected But you provided " << argc - 1 << " arguments";
        exit(0);
    }

    Parser parser;
    CodeWriter codeWriter;
    ExtendedString fName;
    // Making output file to write in codewriter.
    std::string opFileName = inputName;
    if (isFile)
    {
        opFileName = opFileName.substr(0, opFileName.length() - 3);
    }
    codeWriter = CodeWriter(opFileName);

    // actual conversion code.

    for (int i = 0; i < fileNameList.size(); i++)
    {
        fName = fileNameList.getItemByIndex(i);
        parser.setFile(fName);
        std::string fileName = fName.substr(0, fName.length() - 3);
        while (parser.hasMoreCommands())
        {
            parser.advance();
            if (!parser.isWhiteSpaceOrComment())
            {
                ExtendedString error = parser.Error();
                if (error == "")
                {
                    switch (parser.commandType())
                    {
                    case C_ARITHMETIC:
                        codeWriter.writeArithmetic(parser.currentCommand);
                        break;
                    case C_PUSH:
                        codeWriter.writePushPop(C_PUSH, parser.arg1(), std::stoi(parser.arg2()));
                        break;
                    case C_POP:
                        codeWriter.writePushPop(C_POP, parser.arg1(), std::stoi(parser.arg2()));
                        break;
                    case C_LABEL:
                        codeWriter.writeLabel(parser.arg1());
                        break;
                    case C_GOTO:
                        codeWriter.writeGoto(parser.arg1());
                        break;
                    case C_IF:
                        codeWriter.writeIf(parser.arg1());
                        break;
                    case C_FUNCTION:
                        codeWriter.writeFunction(parser.arg1(), std::stoi(parser.arg2()));
                        break;
                    case C_CALL:
                        codeWriter.writeCall(parser.arg1(), std::stoi(parser.arg2()));
                        break;
                    case C_RETURN:
                        codeWriter.writeReturn();
                        break;

                    default:
                        break;
                    }
                    std::cout << parser.currentCommand << std::endl;
                }
                else
                {
                    std::cout << "Error in line " << parser.lineCount << " of file '" << parser.inputFileName << "'" << std::endl
                              << "Error Command: " << parser.currentCommand << std::endl
                              << "Error Message: " << error << std::endl;
                    exit(0);
                }
            }
        }
    }
    std::cout << "Translated Successfully" << std::endl;
    return 0;
}

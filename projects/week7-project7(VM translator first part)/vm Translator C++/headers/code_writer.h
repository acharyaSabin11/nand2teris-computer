#include "extensions.h"
#include <fstream>
#include "enums.h"

class CodeWriter
{
    ExtendedMap segmentPointerMap;
    ExtendedMap operationSymbolMap;
    std::string fileName;
    std::ofstream outputFile;
    ExtendedString outputLine;
    int currentLine = 0;
    void mapInitialize()
    {
        // initalizing the segment pointer map
        segmentPointerMap.addKVPair("local", "LCL");
        segmentPointerMap.addKVPair("argument", "ARG");
        segmentPointerMap.addKVPair("this", "THIS");
        segmentPointerMap.addKVPair("that", "THAT");

        // initializing the operation symbol map
        operationSymbolMap.addKVPair("add", "+");
        operationSymbolMap.addKVPair("sub", "-");
        operationSymbolMap.addKVPair("neg", "-");
        operationSymbolMap.addKVPair("and", "&");
        operationSymbolMap.addKVPair("or", "|");
        operationSymbolMap.addKVPair("not", "!");
        operationSymbolMap.addKVPair("eq", "EQ");
        operationSymbolMap.addKVPair("gt", "GT");
        operationSymbolMap.addKVPair("lt", "LT");
    }

public:
    // constructor
    CodeWriter(std::string outputFileName)
    {
        fileName = outputFileName; // this filename is used name the static pointers
        outputFileName += ".asm";
        mapInitialize();
        outputFile.open(outputFileName);
        outputFile << "// Starter boiler code for conditional checks" << std::endl;
        outputFile << "@START" << std::endl
                   << "0; JMP" << std::endl
                   << "(setTrue)" << std::endl
                   << "@SP" << std::endl
                   << "A=M-1" << std::endl
                   << "M=-1" << std::endl
                   << "@R13" << std::endl
                   << "A=M" << std::endl
                   << "0;JMP" << std::endl
                   << "(setFalse)" << std::endl
                   << "@SP" << std::endl
                   << "A=M-1" << std::endl
                   << "M=0" << std::endl
                   << "@R13" << std::endl
                   << "A=M" << std::endl
                   << "0;JMP" << std::endl;
        // for eq operation
        outputFile << "(EQ)" << std::endl
                   << "@SP" << std::endl
                   << "M=M-1" << std::endl
                   << "A=M" << std::endl
                   << "D=M" << std::endl
                   << "A=A-1" << std::endl
                   << "D=M-D" << std::endl
                   << "@setTrue" << std::endl
                   << "D;JEQ" << std::endl
                   << "@setFalse" << std::endl
                   << "0;JMP" << std::endl;
        // for gt operation
        outputFile << "(GT)" << std::endl
                   << "@SP" << std::endl
                   << "M=M-1" << std::endl
                   << "A=M" << std::endl
                   << "D=M" << std::endl
                   << "A=A-1" << std::endl
                   << "D=M-D" << std::endl
                   << "@setTrue" << std::endl
                   << "D;JGT" << std::endl
                   << "@setFalse" << std::endl
                   << "0;JMP" << std::endl;
        // for lt operation
        outputFile << "(LT)" << std::endl
                   << "@SP" << std::endl
                   << "M=M-1" << std::endl
                   << "A=M" << std::endl
                   << "D=M" << std::endl
                   << "A=A-1" << std::endl
                   << "D=M-D" << std::endl
                   << "@setTrue" << std::endl
                   << "D;JLT" << std::endl
                   << "@setFalse" << std::endl
                   << "0;JMP" << std::endl
                   << "(START)" << std::endl;
        currentLine += 44;
    }

    void writeArithmetic(ExtendedString command)
    {

        outputFile << "// " << command << std::endl;
        if (command == "add" || command == "sub" || command == "and" || command == "or")
        {
            outputFile << "@SP" << std::endl
                       << "M=M-1" << std::endl
                       << "A=M" << std::endl
                       << "D=M" << std::endl
                       << "A=A-1" << std::endl
                       << "M=M" << operationSymbolMap[command] << "D" << std::endl;
            currentLine += 6;
        }
        else if (command == "neg" || command == "not")
        {
            outputFile << "@SP" << std::endl
                       << "A=M-1" << std::endl
                       << "M=" << operationSymbolMap[command] << "M" << std::endl;
            currentLine += 3;
        }
        else if (command == "eq" || command == "gt" || command == "lt")
        {
            currentLine += 6;
            outputFile << "@" << currentLine << std::endl
                       << "D=A" << std::endl
                       << "@R13" << std::endl
                       << "M=D" << std::endl
                       << "@" << operationSymbolMap[command] << std::endl
                       << "0;JMP" << std::endl;
        }
    }

    void writePushPop(CommandType cmdType, ExtendedString segment, int index)
    {
        std::string type = (cmdType == C_PUSH) ? "push " : "pop ";
        outputFile << "// " << type << segment << " " << index << std::endl;
        switch (cmdType)
        {
        case C_PUSH:
            if (segment == "local" || segment == "argument" || segment == "this" || segment == "that")
            {
                // 10 lines of assembly code
                outputFile << "@" << std::to_string(index) << std::endl // if index = 2 , it prints @2
                           << "D=A" << std::endl
                           << "@" << segmentPointerMap[segment] << std::endl // Eg: @LCL
                           << "A=M+D" << std::endl                           // storing the base + index value;
                           << "D=M" << std::endl                             // storing the base + index value;
                           << "@SP" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M+1" << std::endl;
                currentLine += 10;
            }
            else if (segment == "constant")
            {
                outputFile << "@" << std::to_string(index) << std::endl // if index = 2 , it prints @2
                           << "D=A" << std::endl
                           << "@SP" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M+1" << std::endl;
                currentLine += 7;
            }
            else if (segment == "static")
            {
                std::string variableName = "fileName." + std::to_string(index);
                outputFile << "@" << variableName << std::endl //@static.index
                           << "D=M" << std::endl
                           << "@SP" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M+1" << std::endl;
                currentLine += 7;
            }
            else if (segment == "temp")
            {
                // 10 lines of assembly code
                outputFile << "@" << std::to_string(index) << std::endl // if index = 2 , it prints @2
                           << "D=A" << std::endl
                           << "@5" << std::endl    // Eg: @LCL
                           << "A=A+D" << std::endl // storing the base + index value;
                           << "D=M" << std::endl   // storing the base + index value;
                           << "@SP" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M+1" << std::endl;
                currentLine += 10;
            }
            else if (segment == "pointer")
            {
                std::string pointer;
                if (index == 0)
                    pointer = "THIS";
                else if (index == 1)
                    pointer = "THAT";
                outputFile << "@" << pointer << std::endl
                           << "D=M" << std::endl
                           << "@SP" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M+1" << std::endl;
                currentLine += 7;
            }
            break;

        case C_POP:
            if (segment == "local" || segment == "argument" || segment == "this" || segment == "that")
            {
                // 13 lines of assembly code
                outputFile << "@" << std::to_string(index) << std::endl // if index = 2 , it prints @2
                           << "D=A" << std::endl
                           << "@" << segmentPointerMap[segment] << std::endl // Eg: @LCL
                           << "D=M+D" << std::endl                           // storing the base + index value;
                           << "@R13" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M-1" << std::endl
                           << "A=M" << std::endl
                           << "D=M" << std::endl
                           << "@R13" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl;
                currentLine += 13;
            }
            else if (segment == "static")
            {
                std::string variableName = "fileName." + std::to_string(index);
                outputFile << "@SP" << std::endl
                           << "M=M-1" << std::endl
                           << "A=M" << std::endl
                           << "D=M" << std::endl
                           << "@" << variableName << std::endl //@static.index
                           << "M=D" << std::endl;
                currentLine += 6;
            }
            else if (segment == "temp")
            {
                // 13 lines of assembly code
                outputFile << "@" << std::to_string(index) << std::endl // if index = 2 , it prints @2
                           << "D=A" << std::endl
                           << "@5" << std::endl    // Eg: @LCL
                           << "D=A+D" << std::endl // storing the base + index value;
                           << "@R13" << std::endl
                           << "M=D" << std::endl
                           << "@SP" << std::endl
                           << "M=M-1" << std::endl
                           << "A=M" << std::endl
                           << "D=M" << std::endl
                           << "@R13" << std::endl
                           << "A=M" << std::endl
                           << "M=D" << std::endl;
                currentLine += 13;
            }
            else if (segment == "pointer")
            {
                std::string pointer;
                if (index == 0)
                    pointer = "THIS";
                else if (index == 1)
                    pointer = "THAT";
                outputFile
                    << "@SP" << std::endl
                    << "M=M-1" << std::endl
                    << "A=M" << std::endl
                    << "D=M" << std::endl
                    << "@" << pointer << std::endl
                    << "M=D" << std::endl;
                currentLine += 6;
            }
            break;

        default:
            break;
        }
    }
};
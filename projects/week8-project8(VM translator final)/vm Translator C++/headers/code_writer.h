#include "extensions.h"
#include <fstream>
#include "enums.h"

class CodeWriter
{
    ExtendedMap segmentPointerMap;  // to get the segment pointer name based on the code used in vm code.
    ExtendedMap operationSymbolMap; // to provide the operator symbol in assembly based on the word in the vm code.
    std::string fileName;           // output file Name storing variable.
    std::ofstream outputFile;       // output stream file to open file for outputing assembly code.
    int callCount = 0;              // this variable is used to provide a uniqure return label for each call.
    int currentLine = 0;            // this variable is used to keep track of the line where the execution should be returned after implementing the less than, greater than or equal to checks. This is tracked inside the program.
    //! Please be sure to update the current line count if any changes in assembly code is to be made. If the line count don't match, the whole program flow is wasted.

    void mapInitialize() // populating each maps with their values.
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

    void writeInit() // writing the initial code for specifying the stack to start from 256 memory location and calling the sys.init function.
    {
        outputFile << "@256" << std::endl
                   << "D=A" << std::endl
                   << "@SP" << std::endl
                   << "M=D" << std::endl;
        outputFile << "@"
                   << "return" << ++callCount << std::endl
                   << "D=A" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push LCL
                   << "@LCL" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push arg
                   << "@ARG" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push this
                   << "@THIS" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push that
                   << "@THAT" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // Repositioning arg pointer
                   << "@5" << std::endl
                   << "D=A" << std::endl
                   << "@" << 0 << std::endl
                   << "D=A+D" << std::endl
                   << "@SP" << std::endl
                   << "A=M" << std::endl
                   << "D=A-D" << std::endl
                   << "@ARG" << std::endl
                   << "M=D" << std::endl
                   // Repositioning LCL pointer
                   << "@SP" << std::endl
                   << "D=M" << std::endl
                   << "@LCL" << std::endl
                   << "M=D" << std::endl
                   // goto function Name;
                   << "@"
                   << "sys.init" << std::endl
                   << "0;JMP" << std::endl
                   // declare return label
                   << "(return" << callCount << ")" << std::endl;
        currentLine += 49;
    }

public:
    //? default constructor
    CodeWriter() {}
    //? constructor which initialized the file for writing and also writes the boiler code required in each file.
    CodeWriter(std::string outputFileName)
    {
        fileName = outputFileName;       // this filename is used name the static pointers
        outputFileName += ".asm";        // making the ouput file name
        mapInitialize();                 // initializing maps
        outputFile.open(outputFileName); // opening file.
        if (!outputFile.good())          // throwing error if the output file cannot be opened or created.
        {
            std::cout << "Error opening the output file." << std::endl;
            exit(0);
        }
        writeInit();                                                                           // when file is opened, write the initalization code.
        outputFile << "// Starter boiler code for conditional checks and return" << std::endl; // write boiler code for condtional checks and returns.
        outputFile << "(setTrue)" << std::endl
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
                   << "0;JMP" << std::endl; // 12
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
                   << "0;JMP" << std::endl; // 10
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
                   << "0;JMP" << std::endl; // 10
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
                   << "0;JMP" << std::endl; // 10
        // for return operation
        outputFile << "(RETURN)" << std::endl
                   // endframe = LCL
                   << "@LCL" << std::endl
                   << "D=M" << std::endl
                   << "@endFrame" << std::endl
                   << "M=D" << std::endl
                   // retAddr = *(endFrame - 5)
                   << "@5" << std::endl
                   << "A=D-A" << std::endl
                   << "D=M" << std::endl
                   << "@retAddr" << std::endl
                   << "M=D" << std::endl
                   // *ARG = pop()
                   << "@SP" << std::endl
                   << "M=M-1" << std::endl
                   << "A=M" << std::endl
                   << "D=M" << std::endl
                   << "@ARG" << std::endl
                   << "A=M" << std::endl
                   << "M=D" << std::endl
                   // SP = ARG + 1;
                   << "@ARG" << std::endl
                   << "D=M+1" << std::endl
                   << "@SP" << std::endl
                   << "M=D" << std::endl
                   // THAT = *(endFrame - 1)
                   << "@endFrame" << std::endl
                   << "A=M-1" << std::endl
                   << "D=M" << std::endl
                   << "@THAT" << std::endl
                   << "M=D" << std::endl
                   // THIS = *(endFrame - 2)
                   << "@2" << std::endl
                   << "D=A" << std::endl
                   << "@endFrame" << std::endl
                   << "A=M-D" << std::endl
                   << "D=M" << std::endl
                   << "@THIS" << std::endl
                   << "M=D" << std::endl
                   // ARG = *(endFrame - 3)
                   << "@3" << std::endl
                   << "D=A" << std::endl
                   << "@endFrame" << std::endl
                   << "A=M-D" << std::endl
                   << "D=M" << std::endl
                   << "@ARG" << std::endl
                   << "M=D" << std::endl
                   // LCL = *(endFrame - 4)
                   << "@4" << std::endl
                   << "D=A" << std::endl
                   << "@endFrame" << std::endl
                   << "A=M-D" << std::endl
                   << "D=M" << std::endl
                   << "@LCL" << std::endl
                   << "M=D" << std::endl
                   // goto retAddr
                   << "@retAddr" << std::endl
                   << "A=M" << std::endl
                   << "0;JMP" << std::endl;

        currentLine += (42 + 49);
    }

    void writeArithmetic(ExtendedString cmd)
    {
        ExtendedList brokenCommandList = cmd.split(' ');
        ExtendedString command = brokenCommandList.getItemByIndex(0); // using only the first word as arithmetic command as whole instruction can contain comments too.

        outputFile << "// " << command << std::endl; // comment in assembly for easy debugging purpose.
        // writing assembly code based on command.
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

    void writePushPop(CommandType cmdType, ExtendedString segment, int index, ExtendedString inputFileName)
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
                std::string variableName = inputFileName + "." + std::to_string(index);
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
                std::string variableName = inputFileName + "." + std::to_string(index);
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

    void writeLabel(ExtendedString label)
    {
        outputFile << "// label " << label << std::endl
                   << "(" << label << ")" << std::endl;
        currentLine++;
    }

    void writeGoto(ExtendedString label)
    {

        outputFile << "// goto " << label << std::endl
                   << "@" << label << std::endl
                   << "0;JMP" << std::endl;
        currentLine += 2;
    }

    void writeIf(ExtendedString label)
    {
        outputFile << "// if-goto " << label << std::endl
                   << "@SP" << std::endl
                   << "M=M-1" << std::endl
                   << "A=M" << std::endl
                   << "D=M" << std::endl
                   << "@" << label << std::endl
                   << "D;JNE" << std::endl;
        currentLine += 6;
    }

    void writeFunction(ExtendedString functionName, int numVars)
    {
        outputFile << "// function " << functionName << " " << numVars << std::endl
                   << "(" << functionName << ")" << std::endl
                   << "@SP" << std::endl
                   << "A=M" << std::endl;
        // creating local variables for the function
        for (int i = 0; i < numVars; i++)
        {
            outputFile << "M=0" << std::endl
                       << "A=A+1" << std::endl;
        }
        outputFile << "D=A" << std::endl
                   << "@SP" << std::endl
                   << "M=D" << std::endl;
        currentLine += (numVars * 2 + 5);
    }

    void writeCall(ExtendedString functionName, int numArgs)
    { // push return address
        outputFile << "//Call " << functionName << std::endl
                   << "@"
                   << "return" << ++callCount << std::endl
                   << "D=A" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push LCL
                   << "@LCL" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push arg
                   << "@ARG" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push this
                   << "@THIS" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // push that
                   << "@THAT" << std::endl
                   << "D=M" << std::endl
                   << "@SP" << std::endl
                   << "M=M+1" << std::endl
                   << "A=M-1" << std::endl
                   << "M=D" << std::endl
                   // Repositioning arg pointer
                   << "@5" << std::endl
                   << "D=A" << std::endl
                   << "@" << numArgs << std::endl
                   << "D=A+D" << std::endl
                   << "@SP" << std::endl
                   << "A=M" << std::endl
                   << "D=A-D" << std::endl
                   << "@ARG" << std::endl
                   << "M=D" << std::endl
                   // Repositioning LCL pointer
                   << "@SP" << std::endl
                   << "D=M" << std::endl
                   << "@LCL" << std::endl
                   << "M=D" << std::endl
                   // goto function Name;
                   << "@" << functionName << std::endl
                   << "0;JMP" << std::endl
                   // declare return label
                   << "(return" << callCount << ")" << std::endl;
        currentLine += 45;
    }

    void writeReturn()
    {
        outputFile << "// return" << std::endl
                   << "@RETURN" << std::endl
                   << "0;JMP" << std::endl;
        currentLine += 3;
    }
};
#include <iostream>
#include <fstream>
#include <string>
#include "extensions.h"
#include "enums.h"

class Parser
{
    ExtendedList arithmeticInstructionList;
    ExtendedList memoryTypeList;
    void initializeLists()
    {
        // initialize the arithmetic instruction list
        arithmeticInstructionList.add("add");
        arithmeticInstructionList.add("sub");
        arithmeticInstructionList.add("neg");
        arithmeticInstructionList.add("eq");
        arithmeticInstructionList.add("gt");
        arithmeticInstructionList.add("lt");
        arithmeticInstructionList.add("and");
        arithmeticInstructionList.add("or");
        arithmeticInstructionList.add("not");

        // initialize the memory type list.
        memoryTypeList.add("local");
        memoryTypeList.add("argument");
        memoryTypeList.add("this");
        memoryTypeList.add("that");
        memoryTypeList.add("constant");
        memoryTypeList.add("static");
        memoryTypeList.add("pointer");
        memoryTypeList.add("temp");
    }

public:
    std::string inputfileName;
    std::ifstream inputFile;
    bool showTemp;
    int lineCount = 0;
    ExtendedString currentCommand;
    ExtendedList brokenCommandList;

    //? Constructor
    Parser(int argc, char *argv[])
    {
        initializeLists();
        // validating arguments passed and the file name
        if (argc == 2)
        {
            inputfileName = argv[1];
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
        }
        else
        {
            std::cout << "One positional argument (file name)  is expected But you provided " << argc - 1 << " arguments";
            exit(0);
        }
    }

    bool isWhiteSpaceOrComment()
    {
        int forwardSlashCount = 0;
        // if the input string is empty
        if (currentCommand.length() == 0)
        {
            return true;
        }

        // if the input string is not empty
        for (int i = 0; i < currentCommand.length(); i++)
        {
            // if not white space
            if (currentCommand[i] != ' ' && currentCommand[i] != '\t' && currentCommand[i] != '\n')
            {
                // confirmed that the input is not a white space

                // Now, if the character is a / , increment the forwardSlashCount
                if (currentCommand[i] == '/')
                {
                    forwardSlashCount++;
                    if (forwardSlashCount == 2)
                    {
                        return true;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
    }

    bool hasMoreCommands()
    {
        return !inputFile.eof();
    }

    //? advance function: Reads the next command from input and makes it the current command.
    void advance()
    {
        lineCount++;
        std::getline(inputFile, currentCommand);
    }

    CommandType commandType()
    {
        CommandType cmdType = NONE;
        brokenCommandList = currentCommand.split(' ');
        // for arithmetic commands
        if (brokenCommandList.size() == 1)
        {
            if (arithmeticInstructionList.contains(currentCommand))
            {
                cmdType = C_ARITHMETIC;
            }
        }
        // for memory access commands
        if (brokenCommandList.size() == 3)
        {
            if (brokenCommandList.getItemByIndex(0) == "push" || brokenCommandList.getItemByIndex(0) == "pop")
            {
                if (memoryTypeList.contains(brokenCommandList.getItemByIndex(1)))
                {
                    bool result = isOnlyDigits(brokenCommandList.getItemByIndex(2));
                    if (result)
                    {
                        brokenCommandList.getItemByIndex(0) == "push" ? cmdType = C_PUSH : cmdType = C_POP;
                    }
                }
            }
        }
        return cmdType;
    }

    ExtendedString arg1()
    {
        return brokenCommandList.getItemByIndex(1);
    }

    ExtendedString arg2()
    {
        return brokenCommandList.getItemByIndex(2);
    }
};
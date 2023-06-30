#include <iostream>
#include <fstream>
#include <string>
#include "extensions.h"
#include "enums.h"

class Parser
{
    ExtendedList arithmeticInstructionList;
    ExtendedList memoryTypeList;
    ExtendedList branchingInstructionList;
    ExtendedList memoryAccessAndFunctionInstructionList;
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

        // intializing memory access and function instruction list
        memoryAccessAndFunctionInstructionList.add("push");
        memoryAccessAndFunctionInstructionList.add("pop");
        memoryAccessAndFunctionInstructionList.add("function");
        memoryAccessAndFunctionInstructionList.add("call");
        memoryAccessAndFunctionInstructionList.add("return");

        // initializing branching instruction list
        branchingInstructionList.add("goto");
        branchingInstructionList.add("label");
        branchingInstructionList.add("if-goto");
    }

public:
    std::ifstream inputFile;
    bool showTemp;
    int lineCount = 0;
    ExtendedString currentCommand;
    ExtendedList brokenCommandList;
    ExtendedString inputFileName;

    //? default constructor
    Parser()
    {
        initializeLists();
    }

    //? Constructor
    void setFile(ExtendedString fileName)
    {
        inputFileName = fileName;
        inputFile.close();
        inputFile.open(fileName.substr(0, fileName.length()));
        if (!inputFile.good())
        {
            std::cout << "Error opening the file : " << fileName << std::endl;
            exit(0);
        }
        else
        {
            std::cout << "Success Opening the file : " << fileName << std::endl;
        }
        // validating arguments passed and the file name
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
        return false;
    }

    void codeFormatter() // converts the command string into a type that there is only a space between each of word.
    {
        int spaceCount = 0;
        bool commandIntitated = false;
        ExtendedString newCommand = "";
        currentCommand.toLowerCase(); // converts all code to lowecase
        for (char i : currentCommand)
        {
            if (i == '\t' || i == ' ')
            {
                if (spaceCount == 0 && commandIntitated)
                {
                    spaceCount++;
                    newCommand += i;
                }
            }
            else
            {
                spaceCount = 0;
                commandIntitated = true;
                newCommand += i;
            }
        }
        currentCommand = newCommand;
    }

    ExtendedString Error()
    {
        brokenCommandList = currentCommand.split(' ');
        ExtendedString command = brokenCommandList.getItemByIndex(0);
        int commandSize;
        if (arithmeticInstructionList.contains(command) || command == "return")
        {
            commandSize = 1;
        }
        else if (branchingInstructionList.contains(command))
        {
            commandSize = 2;
        }
        else if (memoryAccessAndFunctionInstructionList.contains(command))
        {
            commandSize = 3;
        }
        else
        {
            ExtendedString error = command + " is not a valid command.";
            return "This is not a valid command";
        }

        if (commandSize == 3)
        {
            bool result = isOnlyDigits(brokenCommandList.getItemByIndex(2));
            if (!result)
            {
                return "Push or pop command must have second argument as an integer and '" + brokenCommandList.getItemByIndex(2) + "' is not an intger";
            }
            if (command == "push" || command == "pop")
            {
                if (!memoryTypeList.contains(brokenCommandList.getItemByIndex(1)))
                {
                    return "The name of memory segment '" + brokenCommandList.getItemByIndex(1) + "' is not valid";
                }
            }
        }

        if (brokenCommandList.size() > commandSize)
        {
            ExtendedString probableComment = brokenCommandList.getItemByIndex(commandSize);
            if (probableComment[0] == '/')
            {
                if (probableComment[0] == '/' && probableComment[1] != '/')
                {
                    return "Incomplete Comment";
                }
            }
            else if (probableComment == "")
            {
                return "";
            }
            else
            {
                ExtendedString error = "Invallid token : '" + probableComment + "'";
                return error;
            }
        }

        return "";
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
        codeFormatter();
    }

    CommandType commandType()
    {
        CommandType cmdType = NONE;
        // for arithmetic commands

        if (arithmeticInstructionList.contains(brokenCommandList.getItemByIndex(0)))
        {
            cmdType = C_ARITHMETIC;
        }
        else if (brokenCommandList.getItemByIndex(0) == "label")
        {
            cmdType = C_LABEL;
        }
        else if (brokenCommandList.getItemByIndex(0) == "goto")
        {
            cmdType = C_GOTO;
        }
        else if (brokenCommandList.getItemByIndex(0) == "if-goto")
        {
            cmdType = C_IF;
        }
        else if (brokenCommandList.getItemByIndex(0) == "push")
        {
            cmdType = C_PUSH;
        }
        else if (brokenCommandList.getItemByIndex(0) == "pop")
        {
            cmdType = C_POP;
        }
        else if (brokenCommandList.getItemByIndex(0) == "label")
        {
            cmdType = C_LABEL;
        }
        else if (brokenCommandList.getItemByIndex(0) == "goto")
        {
            cmdType = C_GOTO;
        }
        else if (brokenCommandList.getItemByIndex(0) == "if-goto")
        {
            cmdType = C_IF;
        }
        else if (brokenCommandList.getItemByIndex(0) == "function")
        {
            cmdType = C_FUNCTION;
        }
        else if (brokenCommandList.getItemByIndex(0) == "call")
        {
            cmdType = C_CALL;
        }
        else if (brokenCommandList.getItemByIndex(0) == "return")
        {
            cmdType = C_RETURN;
        }

        return cmdType;
    }

    ExtendedString
    arg1()
    {
        return brokenCommandList.getItemByIndex(1);
    }

    ExtendedString arg2()
    {
        return brokenCommandList.getItemByIndex(2);
    }
};
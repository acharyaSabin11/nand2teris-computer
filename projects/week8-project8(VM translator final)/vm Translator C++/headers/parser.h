#include <iostream>
#include <fstream>
#include <string>
#include "extensions.h"
#include "enums.h"

class Parser
{
    ExtendedList arithmeticInstructionList;              // to store the commands for arithmetic operations.
    ExtendedList memoryTypeList;                         // to store the list of memory types (8 memory types)
    ExtendedList branchingInstructionList;               // to store the commands for branching operations.
    ExtendedList memoryAccessAndFunctionInstructionList; // to store the commands for memory access operations or Function,Call orReturn operations.
    void initializeLists()                               // initializes the list with the values they require.
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
    std::ifstream inputFile;        // input file stream object.
    int lineCount = 0;              // this line count variable helps to provide the line number in which the error occured when displaying the error message.
    ExtendedString currentCommand;  // stores the current command.
    ExtendedList brokenCommandList; // this stores the list of command string parts broken by the space character ' '.
    ExtendedString inputFileName;   // this helps to show the file name while throwing the error.

    //? default constructor
    Parser() // constructor only sets value in the lists.
    {
        initializeLists();
    }

    void setFile(ExtendedString fileName) // gets a new file ready for processing.
    {
        inputFileName = fileName; // saving the inputFileName for error showing pupose.
        inputFile.close();        // closing the previous opened file.
        inputFile.open(fileName); // opening the new file.
        if (!inputFile.good())    // if file is not opened, throw error.
        {
            std::cout << "Error opening the file : " << fileName << std::endl;
            exit(0);
        }
        else // if file is opened successfully, show the file opened.
        {
            std::cout << "Success Opening the file : " << fileName << std::endl;
        }
    }

    bool isWhiteSpaceOrComment() // checks if a line is a whitespace or a comment and returns true if they are a white space or a comment.
    {
        int forwardSlashCount = 0; // for comment checking.
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
                    if (forwardSlashCount == 2) // if two consecutive forward slash are found, return true as it specifies a comment.
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
        int spaceCount = 0;             // to count the number of whitespaces between each words.
        bool commandIntitated = false;  // this is used in the case that the line begins with white spaces and then contains useful things. Since the formateer ignores the whitespace in begining, it needs to keep track if the white space is the white space in begining of line or the white space after the commands are initialized.
        ExtendedString newCommand = ""; // stores the formatted command temporarily.
        currentCommand.toLowerCase();   // converts all code to lowecase
        for (char i : currentCommand)   // iterating through each character of the current command string.
        {
            if (i == '\t' || i == ' ' || i == '\v' || i == '\r' || i == '\f') // if there is any type of white space.
            {
                if (spaceCount == 0 && commandIntitated) // if the command is initiated and space count is zero, add a whitespace and increase space count.
                {
                    spaceCount++;
                    newCommand += ' ';
                }
                // now if the command is not initiated, the white spaces are ignored.
                // Also if the space count is greater than 1 meaning that a space character is already inserted, it doesn't insert another space so there is only one space between each character.
            }
            else
            {
                spaceCount = 0; // if any other character is encountered, the space count is reset, command initiated is set to true and the character is added to the new command.
                commandIntitated = true;
                newCommand += i;
            }
        }
        currentCommand = newCommand; // setting the formatted command to the current command string.
    }

    ExtendedString Error() // returns error if any or return empty string if there is no error.
    {
        brokenCommandList = currentCommand.split(' ');                          // breaking down the command string by space character.
        ExtendedString command = brokenCommandList.getItemByIndex(0);           // taking the first part of command string which is to be a actual command type.
        int commandSize;                                                        // stores the total size of command. Here size means the number of words it requires for the instruction to be complete by specifying its arguments too. Eg: 'add' command has size 1 as its complete instruction is "add" only, 'push' command has size 3 as its complete instruction is of form "push segment number" and so one
        if (arithmeticInstructionList.contains(command) || command == "return") // arithmetic and return command has size 1
        {
            commandSize = 1;
        }
        else if (branchingInstructionList.contains(command)) // branching instructions has size 2
        {
            commandSize = 2;
        }
        else if (memoryAccessAndFunctionInstructionList.contains(command)) // memory access and function instructions has size 3
        {
            commandSize = 3;
        }
        else // if command is not located in any of the instruction list means that the command is invalid and the error message is returned.
        {
            ExtendedString error = command + " is not a valid command.";
            return "This is not a valid command";
        }

        if (commandSize == 3) // if the command size if three, its third part should always be an integer.
        {
            bool result = isOnlyDigits(brokenCommandList.getItemByIndex(2)); // checking if it is only digits or not.
            if (!result)                                                     // if it contains other things than digits, error message is sent.
            {
                return "Push or pop command must have second argument as an integer and '" + brokenCommandList.getItemByIndex(2) + "' is not an intger";
            }
            if (command == "push" || command == "pop") // for push pop commands if the segment type is not valid, error message is to be sent.
            {
                if (!memoryTypeList.contains(brokenCommandList.getItemByIndex(1)))
                {
                    return "The name of memory segment '" + brokenCommandList.getItemByIndex(1) + "' is not valid";
                }
            }
        }

        if (brokenCommandList.size() > commandSize) // if the command line contains more words than it's command size, it means that there is an inline comment or a space at last or the line contains some junk that is to be removed.
        {
            ExtendedString probableComment = brokenCommandList.getItemByIndex(commandSize); // stores the just next word than the number of words required by the command. for push command 4th word is the probable comment.
            if (probableComment[0] == '/')
            {
                if (probableComment[1] != '/') // if the probable comment contains only one forward slash, show error as incomplete comment.
                {
                    return "Incomplete Comment";
                }
            }
            else if (probableComment == "")
            {
                return ""; // if there contains a space at last of the command line, then the list contains an empty string extra than the command size. That part is handled here. This is not an error so empty string is sent as an error.
            }
            else
            {
                ExtendedString error = "Invallid token : '" + probableComment + "'"; // if any character than forward slash is encountered, invalid token error is sent.
                return error;
            }
        }
        // if any of the above exception is not thrown, then the code is valid so returning the empty string as erorr which means that there is no error.
        return "";
    }

    bool hasMoreCommands() // returns has more commands until end of file is reached.
    {
        return !inputFile.eof();
    }

    // advance function: Reads the next command from input and makes it the current command.
    void advance()
    {
        lineCount++; // to keep track of which line is being processed.
        std::getline(inputFile, currentCommand);
        codeFormatter();
    }

    CommandType commandType() // determines the type of command required by the code writer.
    {
        CommandType cmdType = NONE; // default
        // determining the type of command based on the first word of the instruction.
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
    { // returning the first argument of the command
        return brokenCommandList.getItemByIndex(1);
    }

    ExtendedString arg2()
    { // returning the second argument of the command.
        return brokenCommandList.getItemByIndex(2);
    }
};
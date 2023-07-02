#include <iostream> //To work with input and output
#include <fstream>  //To work with files
#include <string>   //To work with strings
#include "headers/helpers.h"
#include "headers/parser.h"
#include "headers/code_writer.h"
#include <filesystem>

// namespace shortcut definiton
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    ExtendedList fileNameList; // to store the name list of file in case of a folder is given for compilation. If a file is given, the list contains only one file and if the folder is given then only it contains all the files in the folder.
    bool isFile = true;        // to store the identification that the provided argument represents a folder name or a file name.
    std::ifstream inputFile;   // to open a file.
    char *inputName = argv[1]; // to store the argument passed for exe call (i.e. folder name or file name)
    if (argc == 2)             // if only one argument is provided for exe call, it's valid.
    {
        // trying to open the file considering it to be a folder. If it is not a folder exception is thrown. which is handled and then it is checked for to be a vm file.
        try
        {
            for (const auto &entry : fs::directory_iterator(inputName))
            {
                ExtendedString fileName = entry.path().u8string();
                if (fileName.substr(fileName.length() - 3, fileName.length()) == ".vm") // only save the fileName with extension '.vm' to the fileNameList.
                {
                    fileNameList.add(entry.path().u8string());
                }
            }
            isFile = false; // if the directory iteration runs successfully, the isFile flag is set to false. It is a file and can't be iterated, it throws an exception and then the default value of isFalse i.e. true is remained.
        }
        catch (...)
        {
            // this block is reached only if the given argument is a file name. Now handling all the file related issues here.
            std::string inputfileName = inputName; // variable to store the file name provided.
            inputFile.open(inputfileName);         // opening the file.
            if (!inputFile)                        // if file can't be opened, it means that the file doesn't exist.
            {
                std::cout << "Provided file doesn't exist";
                exit(0);
            }
            if (inputfileName.substr(inputfileName.length() - 3, inputfileName.length()) != ".vm") // if file is opened, it means it exists but if it's extension is not '.vm', erroe is to be shown.
            {
                std::cout << "The provided file is not of extension '.vm'";
                inputFile.close();
                exit(0);
            }
            else // if everything is okay with file, then adding the file name to the fileNameList.
            {
                fileNameList.add(inputfileName);
                inputFile.close();
            }
        }
    }
    else // if no argument or more than 2 argument is provided for exe call, show the error.
    {
        std::cout << "One positional argument (file name) is expected But you provided " << argc - 1 << " arguments";
        exit(0);
    }

    Parser parser;         // this object parses commands from each file, formats the string, determines the command type, checks for syntax errors and provides the arguments required to write code.
    CodeWriter codeWriter; // this object solely is dedicated to write the code.
    ExtendedString fName;  // variable to store the name of file set for current reading.
    // Making output file to write in codewriter.
    std::string opFileName = inputName;                   // variable to store the name of the output file. initially, it is set to the value of the argument passed to the main function i.e. while executing the exe.
    ExtendedString fileNameWithoutExtension = opFileName; // this is required for the name to be used while creating the name of the static variables which requires the name of the current file without extension to have it's name.
    if (isFile)                                           // if the argument represents a file, then simply removing the extension '.vm provides the output file name'
    {
        opFileName = opFileName.substr(0, opFileName.length() - 3);
    }
    // if the arugment represents the folder, then the inputName value set as default to the opFileName is the name of output file which is the same as folder name.
    codeWriter = CodeWriter(opFileName); // calling the codeWriter constructor with file name to make the output file ready for outputing the assembly code.

    // actual conversion code.
    for (int i = 0; i < fileNameList.size(); i++) // iterating through all the name files in the fileNameList.
    {
        fName = fileNameList.getItemByIndex(i); // getting a file in each iteration.
        if (isFile)                             // if only a file was provided, the file name without extension is just the name without '.vm' extension.
        {
            fileNameWithoutExtension = fName.substr(0, fName.length() - 3);
        }
        else // if the folder was provided, the file name is stored in the fileNameList as "folderName//fileName.extension" so we need to remove the "folderName//" and ".extension" part. here extension part is '.vm' for each file.
        {
            // opFileName contains the folder name. +1 is done to include the "//" part too as 0 to opFileName.length() + 1 includes the while "folderName//" part.
            // Another part is used to calculate the offset to be covered for substr which is calcuated by deducting the length of the "folderName//" and ".vm" from the fileName length.
            fileNameWithoutExtension = fName.substr(opFileName.length() + 1, fName.length() - (opFileName.length() + 1) - 3);
        }

        parser.setFile(fName); // setting the current file to be parsed.

        while (parser.hasMoreCommands()) // iterating through each line of the file.
        {
            parser.advance();                    // getting next line of the file.
            if (!parser.isWhiteSpaceOrComment()) // conversion is to be done if the line is not a whitespace or comment.
            {
                ExtendedString error = parser.Error(); // checking error.
                if (error == "")                       // if there is no error, then only the code is to be sent for code writing.
                {
                    switch (parser.commandType()) // writing the assembly code based on the type of vm command.
                    {
                    case C_ARITHMETIC:
                        codeWriter.writeArithmetic(parser.currentCommand);
                        break;
                    case C_PUSH:
                        codeWriter.writePushPop(C_PUSH, parser.arg1(), std::stoi(parser.arg2()), fileNameWithoutExtension);
                        break;
                    case C_POP:
                        codeWriter.writePushPop(C_POP, parser.arg1(), std::stoi(parser.arg2()), fileNameWithoutExtension);
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
                    std::cout << parser.currentCommand << std::endl; // showing all the code iterated through.
                }
                else // encountering any error shows the erroe with line number in which the error occured, file Name in which the error occured, the whole command in which the error occured and the error message specifying what went wrong.
                {
                    std::cout << "Error in line " << parser.lineCount << " of file '" << parser.inputFileName << "'" << std::endl
                              << "Error Command: " << parser.currentCommand << std::endl
                              << "Error Message: " << error << std::endl;
                    exit(0);
                }
            }
        }
    }
    std::cout << "Translated Successfully" << std::endl; // This message is shown if the transaltion of the code is successful.
    return 0;
}

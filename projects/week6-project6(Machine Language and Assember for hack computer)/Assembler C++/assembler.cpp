#include <iostream> //To work with input and output
#include <fstream>  //To work with files
#include <string>   //To work with strings
#include "headers/extensions.h"
#include "headers/helpers.h"

int main(int argc, char *argv[])
{
    bool showTemp;
    int lineCount = 0;
    std::ifstream inputFile;
    // validating arguments passed and the file name
    std::string inputfileName;
    std::string outputfileName;
    if (argc == 2 || argc == 3)
    {
        inputfileName = argv[1];
        inputFile.open(inputfileName);
        if (!inputFile)
        {
            std::cout << "Provided file doesn't exist";
            exit(0);
        }
        if (inputfileName.substr(inputfileName.length() - 4, inputfileName.length()) != ".asm")
        {
            std::cout << "The provided file is not of extension '.asm'";
            exit(0);
        }
        // opening the input asm file to read.
        outputfileName = inputfileName.substr(0, inputfileName.length() - 4) + ".hack";
        // if flag is provided, it should be valid
        if (argc == 3)
        {
            if (std::string(argv[2]) == "--showTemp")
            {
                showTemp = true;
            }
            else
            {
                std::cout << "Invalid Flag '" << argv[2] << "' provided.";
                exit(0);
            }
        }
    }
    else
    {
        std::cout << "One or Two positional arguments (file name and flag)  is expected But you provided " << argc - 1 << " arguments";
        exit(0);
    }

    // Making a map to store the instruction of machine
    ExtendedMap instructionMap;
    instructionMap.addKVPair("0", "0101010");
    instructionMap.addKVPair("1", "0111111");
    instructionMap.addKVPair("-1", "0111010");
    instructionMap.addKVPair("D", "0001100");
    instructionMap.addKVPair("A", "0110000");
    instructionMap.addKVPair("M", "1110000");
    instructionMap.addKVPair("!D", "0001101");
    instructionMap.addKVPair("!A", "0110001");
    instructionMap.addKVPair("!M", "1110001");
    instructionMap.addKVPair("-D", "0001111");
    instructionMap.addKVPair("-A", "0110011");
    instructionMap.addKVPair("-M", "1110011");
    instructionMap.addKVPair("D+1", "0011111");
    instructionMap.addKVPair("A+1", "0110111");
    instructionMap.addKVPair("M+1", "1110111");
    instructionMap.addKVPair("D-1", "0001110");
    instructionMap.addKVPair("A-1", "0110010");
    instructionMap.addKVPair("M-1", "1110010");
    instructionMap.addKVPair("D+A", "0000010");
    instructionMap.addKVPair("D+M", "1000010");
    instructionMap.addKVPair("D-A", "0010011");
    instructionMap.addKVPair("D-M", "1010011");
    instructionMap.addKVPair("A-D", "0000111");
    instructionMap.addKVPair("M-D", "1000111");
    instructionMap.addKVPair("D&A", "0000000");
    instructionMap.addKVPair("D&M", "1000000");
    instructionMap.addKVPair("D|A", "0010101");
    instructionMap.addKVPair("D|M", "1010101");

    // to store the destination values
    ExtendedMap destinationMap;
    destinationMap.addKVPair("null", "000");
    destinationMap.addKVPair("M", "001");
    destinationMap.addKVPair("D", "010");
    destinationMap.addKVPair("MD", "011");
    destinationMap.addKVPair("A", "100");
    destinationMap.addKVPair("AM", "101");
    destinationMap.addKVPair("AD", "110");
    destinationMap.addKVPair("AMD", "111");

    // to sore the jump values
    ExtendedMap jumpMap;
    jumpMap.addKVPair("null", "000");
    jumpMap.addKVPair("JGT", "001");
    jumpMap.addKVPair("JEQ", "010");
    jumpMap.addKVPair("JGE", "011");
    jumpMap.addKVPair("JLT", "100");
    jumpMap.addKVPair("JNE", "101");
    jumpMap.addKVPair("JLE", "110");
    jumpMap.addKVPair("JMP", "111");

    // symbol table
    ExtendedMap symbolTable;
    // adding predefined symbols
    symbolTable.addKVPair("R0", "000000000000000");
    symbolTable.addKVPair("R1", "000000000000001");
    symbolTable.addKVPair("R2", "000000000000010");
    symbolTable.addKVPair("R3", "000000000000011");
    symbolTable.addKVPair("R4", "000000000000100");
    symbolTable.addKVPair("R5", "000000000000101");
    symbolTable.addKVPair("R6", "000000000000110");
    symbolTable.addKVPair("R7", "000000000000111");
    symbolTable.addKVPair("R8", "000000000001000");
    symbolTable.addKVPair("R9", "000000000001001");
    symbolTable.addKVPair("R10", "000000000001010");
    symbolTable.addKVPair("R11", "000000000001011");
    symbolTable.addKVPair("R12", "000000000001100");
    symbolTable.addKVPair("R13", "000000000001101");
    symbolTable.addKVPair("R14", "000000000001110");
    symbolTable.addKVPair("R15", "000000000001111");
    symbolTable.addKVPair("SCREEN", "100000000000000");
    symbolTable.addKVPair("KBD", "110000000000000");
    symbolTable.addKVPair("SP", "000000000000000");
    symbolTable.addKVPair("LCL", "000000000000001");
    symbolTable.addKVPair("ARG", "000000000000010");
    symbolTable.addKVPair("THIS", "000000000000011");
    symbolTable.addKVPair("THAT", "000000000000100");

    // list to store the used memory value;
    int memoryTracker = 16;
    ExtendedList usedMemoryList;

    // string to read from the asm file.
    ExtendedString inputLine;
    // string to write into output file.
    ExtendedString outputLine;

    // reading from the input file, validating the code and storing the extracted code in a temporary file. It also checks for the validitty of the code.
    std::ofstream extractedCodefile("extractedCode.temp");
    while (std::getline(inputFile, inputLine))
    {
        ++lineCount;
        ExtendedMap codeMap = codeExtractor(inputLine, lineCount, instructionMap, destinationMap, jumpMap);
        ExtendedString code = codeMap["Code"];
        ExtendedString error = codeMap["Error"];
        if (error != "")
        {
            std::cout << error << std::endl;
            exit(1);
        }
        if (code == "")
            continue;
        extractedCodefile << code << std::endl;
    }

    lineCount = 0;
    inputFile.close();
    extractedCodefile.close();

    std::ifstream extractedCode("extractedCode.temp");
    std::ofstream secondLastCode("finalDraft.temp");
    // First pass(LABEL RECORD and removal)
    while (std::getline(extractedCode, inputLine))
    { // records the label and outputs the code without label.
        ExtendedString out = labelRecorder(inputLine, &lineCount, &symbolTable);
        if (out != "")
        {
            secondLastCode << out << std::endl;
        }
    }
    extractedCode.close();
    secondLastCode.close();

    // Second pass(Conversion phase and variable adding phase);
    std::ifstream finalReadFile("finalDraft.temp");
    std::ofstream outputFile(outputfileName);

    while (std::getline(finalReadFile, inputLine))
    {
        ExtendedString finalOutput = secondPass(inputLine, instructionMap, destinationMap, jumpMap, &symbolTable, &usedMemoryList, &memoryTracker);
        outputFile << finalOutput << std::endl;
    }

    finalReadFile.close();
    outputFile.close();

    // printing into file for debugging.
    std::ofstream mapFiles("maps.temp");
    mapFiles << "INSTRUCTION MAP" << std::endl;
    instructionMap.printToFile(mapFiles);
    mapFiles << std::endl
             << std::endl
             << std::endl;
    mapFiles << "JUMP MAP" << std::endl;
    jumpMap.printToFile(mapFiles);
    mapFiles << std::endl
             << std::endl
             << std::endl;
    mapFiles << "DESTINATION MAP" << std::endl;
    destinationMap.printToFile(mapFiles);
    mapFiles << std::endl
             << std::endl
             << std::endl;
    mapFiles << "SYMBOL TABLE" << std::endl;
    symbolTable.printToFile(mapFiles);
    mapFiles.close();

    // removing the temporary file if showTemp is disabled;
    if (!showTemp)
    {
        remove("extractedCode.temp");
        remove("finalDraft.temp");
        remove("maps.temp");
    }

    return 0;
}

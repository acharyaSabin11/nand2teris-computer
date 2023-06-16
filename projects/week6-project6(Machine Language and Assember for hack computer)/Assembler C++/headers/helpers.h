#pragma once
#include <map>
#include "extensions.h"

ExtendedString decimalToBinaryString(int num)
{
    ExtendedString binary = "", binaryToBeReturned = "000000000000000";
    for (int i = 0; num != 0; i++)
    {
        binary += std::to_string(num % 2);
        num /= 2;
    }
    binary.reverse();
    for (int i = 0; i < binary.size(); i++)
    {
        binaryToBeReturned[15 - binary.size() + i] = binary[i];
    }
    return binaryToBeReturned;
}

// function declaration
ExtendedMap codeExtractor(ExtendedString inLine, int ln, ExtendedMap instructionMap, ExtendedMap destinationMap, ExtendedMap jumpMap);
bool isDigit(char ch);
ExtendedMap codeValidator(ExtendedString inCode, ExtendedMap instructionMap, ExtendedMap destinationMap, ExtendedMap jumpMap);

ExtendedMap codeExtractor(ExtendedString inLine, int ln, ExtendedMap instructionMap, ExtendedMap destinationMap, ExtendedMap jumpMap)
{
    std::string lineNumber = std::to_string(ln);
    int lineLength = inLine.length();
    int forwardSlashCount = 0;
    ExtendedMap outputMap;
    // to store the output generated code.
    ExtendedString code = "";

    // if the input string is empty
    if (inLine.length() == 0)
    {
        outputMap.addKVPair("Empty", "Line is Empty");
        return outputMap;
    }

    // if the input string is not empty
    for (int i = 0; i < lineLength; i++)
    {
        // if not white space
        if (inLine[i] != ' ' && inLine[i] != '\t' && inLine[i] != '\n')
        {
            // confirmed that the input is not a white space

            // Now, if the character is a / , increment the forwardSlashCount
            if (inLine[i] == '/')
            {
                forwardSlashCount++;
                // if the code is empty and symbol of // is encountered return that the line is a comment
                if (forwardSlashCount == 2 && code.length() == 0)
                {
                    outputMap.addKVPair("Comment", "Line contains comment");
                    return outputMap;
                }
                // if the code is not empty and symbol of // is encountered , exit the loop and return the code
                else if (forwardSlashCount == 2 && code.length() != 0)
                    break;
            }
            else
            {
                // if the character is not a / and the forwardSlashCount is 1 then return error because it is of form "/*" where * is any character except /
                if (forwardSlashCount == 1)
                {
                    std::string errorMessage;
                    errorMessage += "Line ";
                    errorMessage += lineNumber;
                    errorMessage += ": You have an incomplete syntax for comment";
                    outputMap.addKVPair("Error", errorMessage);
                    return outputMap;
                }
                else
                {
                    code += inLine[i];
                }
            }
        }
    }

    // handling the condition that the line only contains white spaces
    if (code.length() == 0)
    {
        outputMap.addKVPair("Empty", "Line is Empty");
        return outputMap;
    }
    else
    {
        // checking if the code is valid or not.
        ExtendedMap output = codeValidator(code, instructionMap, destinationMap, jumpMap);
        if (output.containsKey("Error"))
        {
            std::string errorMessage;
            errorMessage += "Line ";
            errorMessage += lineNumber;
            errorMessage += ": ";
            errorMessage += output["Error"];
            outputMap.addKVPair("Error", errorMessage);
            return outputMap;
        }
        else if (output.containsKey("Code"))
        {
            outputMap.addKVPair("Code", output["Code"]);
            return outputMap;
        }
        outputMap.addKVPair("Code", code);
        return outputMap;
    }
}

bool isDigit(char ch)
{
    // comparing the ascii code to determine whether the character is a digit or not
    for (int i = 48; i <= 57; i++)
    {
        if (ch == i)
        {
            return true;
        }
    }
    return false;
}

ExtendedMap codeValidator(ExtendedString inCode, ExtendedMap instructionMap, ExtendedMap destinationMap, ExtendedMap jumpMap)
{
    ExtendedMap valueMap;
    int length = inCode.length();
    // checking the a type instruction
    if (inCode[0] == '@')
    {
        ExtendedString substr = inCode.substr(1, length - 1);
        if (isdigit(inCode[1]))
        {
            for (int i = 2; i < length; i++)
            {
                if (!isdigit(inCode[i]))
                {
                    valueMap.addKVPair("Error", "Cannot have an identifier starting with number");
                    return valueMap;
                }
            }
        }
    }
    // checking the c type instruction
    else
    {
        // splitting the c type instruction by equal to and cheking if the left part of the = is valid or not.
        ExtendedList equalToSplittedList = inCode.split('=');
        ExtendedString stringToBeSentForSplitting;
        bool hasEqualTo = false;

        if (equalToSplittedList.size() == 1)
        {
            stringToBeSentForSplitting = inCode;
        }
        else if (equalToSplittedList.size() == 2)
        {
            hasEqualTo = true;
            // check if the left part of equal to was a valid syntax or not
            if (equalToSplittedList.getItemByIndex(0) == "null")
            {
                valueMap.addKVPair("Error", "null is not a valid destination");
                return valueMap;
            }
            else
            {
                if (!destinationMap.containsKey(equalToSplittedList.getItemByIndex(0)))
                {
                    ExtendedString errorString = "";
                    errorString += equalToSplittedList.getItemByIndex(0);
                    errorString += " is not a valid destination";
                    valueMap.addKVPair("Error", errorString);
                    return valueMap;
                }
            }
            // then move the right part to be splitted on basis of ';'
            stringToBeSentForSplitting = equalToSplittedList.getItemByIndex(1);
            // std::advance
        }
        else
        {
            valueMap.addKVPair("Error", "Cannot have two equal twos in a statement.");
            return valueMap;
        }

        // now splitting the code on the basis of ";"
        ExtendedList semicolonSplittedCode = stringToBeSentForSplitting.split(';');
        if (hasEqualTo == true)
        {
            if (semicolonSplittedCode.size() == 1)
            {
                ExtendedString instruction = semicolonSplittedCode.getItemByIndex(0);
                if (!instructionMap.containsKey(instruction))
                {
                    ExtendedString errorString = "";
                    errorString += equalToSplittedList.getItemByIndex(0);
                    errorString += " is not a valid instruction";
                    valueMap.addKVPair("Error", errorString);
                    return valueMap;
                }
            }
        }
        else
        {
            // if the code doesnt contain the equalto or semicolor, it can be either a label or an error code.
            if (semicolonSplittedCode.size() == 1)
            {
                ExtendedString labelOrErrorCode =
                    semicolonSplittedCode.getItemByIndex(0);
                ExtendedString label = "";
                bool isLabel = false;
                if (labelOrErrorCode[0] == '(')
                {
                    for (int i = 1; i < labelOrErrorCode.length(); i++)
                    {
                        if (labelOrErrorCode[i] == ')')
                        {
                            isLabel = true;
                            break;
                        }
                        else
                        {
                            label += labelOrErrorCode[i];
                        }
                    }
                }

                if (isLabel)
                {
                    ExtendedString outputLabel = "(";
                    outputLabel += label;
                    outputLabel += ")";
                    valueMap.addKVPair("Code", outputLabel);
                    return valueMap;
                }
                else
                {
                    ExtendedString errorString = "";
                    errorString += labelOrErrorCode;
                    errorString += " is not a valid assembly code";
                    valueMap.addKVPair("Error", errorString);
                    return valueMap;
                }
            }
            // if file contains semicolor but not equalto
            if (semicolonSplittedCode.size() == 2)
            {
                // if there is nothing after semicolon
                if (semicolonSplittedCode.getItemByIndex(1) == "null")
                {
                    valueMap.addKVPair("Error", "null is not a valid syntax");
                    return valueMap;
                }
                else if (semicolonSplittedCode.getItemByIndex(1) == "")
                {
                    valueMap.addKVPair("Error", "There must be something after ';'");
                    return valueMap;
                }
                else
                {
                    if (!jumpMap.containsKey(semicolonSplittedCode.getItemByIndex(1)))
                    {
                        ExtendedString errorString = "";
                        errorString += semicolonSplittedCode.getItemByIndex(1);
                        errorString += " is not a valid jump statement";
                        valueMap.addKVPair("Error", errorString);
                        return valueMap;
                    }
                }
                if (!instructionMap.containsKey(semicolonSplittedCode.getItemByIndex(0)))
                {
                    ExtendedString errorString = "";
                    errorString += semicolonSplittedCode.getItemByIndex(0);
                    errorString += " is not a valid instruction";
                    valueMap.addKVPair("Error", errorString);
                    return valueMap;
                }
            }
        }
    }
    return valueMap;
}

ExtendedString labelRecorder(ExtendedString code, int *lineNumber, ExtendedMap *symbolTable)
{

    ExtendedString label;
    if (code[0] == '(')
    {
        label = code.substr(1, code.length() - 2);
        symbolTable->addKVPair(label, decimalToBinaryString(*lineNumber));
        return "";
    }
    else
    {
        (*lineNumber)++;
        return code;
    }
}

ExtendedString secondPass(ExtendedString inCode, ExtendedMap instructionMap, ExtendedMap destinationMap, ExtendedMap jumpMap, ExtendedMap *symbolTable, ExtendedList *usedMemoryList, int *memoryTracker)
{
    ExtendedMap valueMap;
    int length = inCode.length();
    // checking the a type instruction
    if (inCode[0] == '@')
    {
        ExtendedString value;
        ExtendedString substr = inCode.substr(1, length - 1);
        if (!isdigit(inCode[1]))
        {
            if (!symbolTable->containsKey(substr))
            {
                // this checking is done to see if the user has used that location from other point or not.
                while (usedMemoryList->contains(std::to_string(*memoryTracker)))
                {
                    (*memoryTracker)++;
                }
                value = decimalToBinaryString(*memoryTracker);
                symbolTable->addKVPair(substr, value);
                usedMemoryList->add(std::to_string(*memoryTracker));
                (*memoryTracker)++;
            }
            else
            {
                value = (*symbolTable)[substr];
            }
            // converting to code
            ExtendedString returnValue = "0";
            returnValue += value;
            return returnValue;
        }
        else
        {
            ExtendedString returnValue = "0";
            value = decimalToBinaryString(std::stoi(substr));
            if (!usedMemoryList->contains(value))
            {
                usedMemoryList->add(value);
            }
            returnValue += value;
            return returnValue;
        }
    }
    // checking the c type instruction
    else
    {
        // splitting the c type instruction by equal to and cheking if the left part of the = is valid or not.
        ExtendedList equalToSplittedList = inCode.split('=');
        ExtendedString stringToBeSentForSplitting;
        ExtendedString dest = "000";
        ExtendedString jump = "000";
        ExtendedString inst;

        // taking care of destination bits
        if (equalToSplittedList.size() == 1)
        {
            stringToBeSentForSplitting = inCode;
        }
        else if (equalToSplittedList.size() == 2)
        {
            dest = destinationMap[equalToSplittedList.getItemByIndex(0)];
            // to further split by ';'
            stringToBeSentForSplitting = equalToSplittedList.getItemByIndex(1);
        }

        // now splitting the code on the basis of ";"
        ExtendedList semicolonSplittedCode = stringToBeSentForSplitting.split(';');
        // getting isntruction bits
        inst = instructionMap[semicolonSplittedCode.getItemByIndex(0)];
        if (semicolonSplittedCode.size() == 2)
        {
            jump = jumpMap[semicolonSplittedCode.getItemByIndex(1)];
        }

        ExtendedString finalInstruction = "111";
        finalInstruction += inst;
        finalInstruction += dest;
        finalInstruction += jump;
        return finalInstruction;
    }
}
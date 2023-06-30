#pragma once
#include <string>
#include <map>
#include <iostream>
#include <list>
#include <fstream>

// declaration of classes
class ExtendedString;
class ExtendedMap;
class ExtendedList;

class ExtendedString : public std::string
{
public:
    // Default class constructor
    ExtendedString() : std::string() {}
    // Constructor accepting a const char* argument
    ExtendedString(const char *str) : std::string(str) {}
    ExtendedString(std::string str) : std::string(str) {}

    std::list<ExtendedString> split(char splitCharacter)
    {
        std::list<ExtendedString> strList;
        ExtendedString strToBeAdded = "";
        for (int i = 0; i < this->length(); i++)
        {
            if ((*this)[i] != splitCharacter)
            {
                strToBeAdded += (*this)[i];
            }
            else
            {
                strList.push_back(strToBeAdded);
                strToBeAdded = "";
            }
        }
        strList.push_back(strToBeAdded);
        return strList;
    }

    void reverse()
    {
        std::string temp;
        for (int i = (*this).length() - 1; i >= 0; i--)
        {
            temp += (*this)[i];
        }
        (*this) = temp;
    }

    void toLowerCase()
    {
        char character;
        for (int i = 0; i < (*this).length(); i++)
        {
            character = (*this)[i];
            if (character > 64 && character < 90)
            {
                (*this)[i] += 32;
            }
        }
    }
};

class ExtendedMap : public std::map<ExtendedString, ExtendedString>
{
public:
    bool containsKey(ExtendedString key)
    {
        std::map<ExtendedString, ExtendedString>::iterator iter = this->begin();
        while (iter != this->end())
        {
            if (iter->first == key)
            {
                return true;
            }
            ++iter;
        }
        return false;
    }

    void display()
    {
        std::map<ExtendedString, ExtendedString>::iterator iter = this->begin();
        while (iter != this->end())
        {
            std::cout << "Key: " << iter->first << ", Value: " << iter->second << std::endl;
            ++iter;
        }
    }

    void addKVPair(ExtendedString key, ExtendedString value)
    {
        (*this)[key] = value;
    }

    void printToFile(std::ofstream &file)
    {
        std::map<ExtendedString, ExtendedString>::iterator iter = this->begin();
        while (iter != this->end())
        {
            file << "Key: " << iter->first << ", Value: " << iter->second << std::endl;
            ++iter;
        }
    }
};

class ExtendedList : public std::list<ExtendedString>
{

public:
    // default constructor
    ExtendedList() {}
    ExtendedList(std::list<ExtendedString> givenList)
    {
        for (auto i : givenList)
        {
            this->add(i);
        }
    }

    ExtendedString getItemByIndex(int index)
    {
        if (index < 0 || index >= this->size())
        {
            std::cout << "Error: Index " << index << " is not in range";
            exit(1);
        }
        ExtendedList::iterator iter = this->begin();
        for (int i = 0; i < index; i++)
        {
            iter++;
        }
        return *iter;
    }

    void add(ExtendedString str)
    {
        this->push_back(str);
    }

    void display()
    {
        for (auto i : (*this))
        {
            std::cout << i << std::endl;
        }
    }

    bool contains(ExtendedString str)
    {
        for (auto i : (*this))
        {
            if (i == str)
            {
                return true;
            }
        }
        return false;
    }
};
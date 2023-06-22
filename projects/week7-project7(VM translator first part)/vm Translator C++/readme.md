This is the implementation of hack vm translator in C++ language. It is a part of nandToTetris course practice from coursera.

To compile the file.
1. Get a c++ compiler. (mingw g++ compiler was used during the development period)
2. Compile the cpp program to get the executable.NOTE: the headers folder should present in this directory as it contains the helper functions and extended classes required by the vmTranslator.cpp file.


To run the execultable.
1. run "vmTranslator fileName " to get the 'asm' file.
    -> fileName having the '.vm' extension is only accepted by the vmTranslator.


IMPORTANT THINGS RELATED TO VM TRANSLATOR: 
1. The vmTranslator assumes that the code is written correct and doesnt check any errors in vm code.
2. The vmTranslator only support a full line comment but not the inline comment. If a line contains any other thing than a pure code, it will just ignore the line and continue with the execution of the other line of code.
3. If the code contains any other ' ' character than required in the code. it will assume the code to be invalid.

NOTE:
The checking feature will be implemented soon and will be updated in this repository only.

*******************************************ALSO ANY SUGGESTIONS AND IMPROVEMENTS ARE ALWAYS APPRECIATED**************************************************
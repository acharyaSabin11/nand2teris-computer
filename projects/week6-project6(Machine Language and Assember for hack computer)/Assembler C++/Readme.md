This is the implementation of hack assembler in C++ language. It is a part of nandToTetris course practice from coursera.

To assemble the file.
1. Get a c++ compiler. (mingw g++ compiler was used during the development period)
2. Compile the cpp program to get the executable.NOTE: the headers folder should present in this directory as it contains the helper functions and extended classes required by the assembler.cpp file.

To run the execultable.
1. run "assembler fileName " to get the binary file.
    fileName having the '.asm' extension is only accepted by the assembler.
2. To get the acces to temporary files generated during the process.
    Run : "assembler fileName --showTemp"
    This outputs 3 additional files:
    a. extractedCode.temp: this contains the pure code without the comments and whitespaces.
    b. finalDraft.temp: this contains the code without labels too.
    c. maps.temp : This contains the map of the instruction set, jump set, destination set used by the hack computer and also the symbol table to see which symbol is allocated which address in the memory.
3. If any error occurs during the assembler process, it is outputted in the console along with the line number in which the error occured and the error that occured.

NOTE: the code is not well optimized for fast execution as it keeps on passing the whole maps and lists for each line of code. This will be an area of future optimization. If the code is optimized in future, it will be updated in this repo only.

*******************************************ALSO ANY SUGGESTIONS AND IMPROVEMENTS ARE ALWAYS APPRECIATED**************************************************
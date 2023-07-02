This is the implementation of hack vm translator in C++ language. It is a part of nandToTetris course practice from coursera. This is the full version of VM translator, a completed version of the one in week 7.

To compile the file.
1. Get a c++ compiler. (mingw g++ compiler was used during the development period) c++ 17 is must as it is required to use the filesystem library.
2. Compile the cpp program to get the executable.NOTE: the headers folder should present in this directory as it contains the helper functions and extended classes required by the vmTranslator.cpp file.


To run the execultable.
1. run "vmTranslator fileName " to get the 'asm' file.
    -> fileName having the '.vm' extension is only accepted by the vmTranslator.
2. To compile a whole folder with multiple vm files.
    -> Be in the directory where the folder is located. Your vmTranslator.exe should also be there or you can just put the path to vmTranslator.exe in the environment variables path.
    -> run 'vmTranslator folderName' and get the 'asm' file named after the folder provided. The asm file is made outputted in the same folder as of the folder name provided.
    -> If the folder is empty, then also an 'asm' file is created but it only contains the assembly code required for initialization in every program. This file cannot be executed.


IMPORTANT THINGS RELATED TO VM TRANSLATOR: 
1. The vmTranslator recognizes the errors in code and throws exception for syntax errors.
2. The vmTranslator support a full line comment & inline comment.
3. The vmTranslator calles the sys.init function first. So if your program doesnt contain the sys.init function then don't expect the code to produce the desired result as it will not know where to execute the program flow.(MUST READ)


NOTE: 
The program ran completing the tests provided which contains the sys.init function. So i have thus considered the vm translator to be correct. But if any future issues occur in the VM translator I will update the code.
Also the code is not so optimized for call instruction as it produces around 50 lines of code for each call. I tried to implement a routine for call so that the repeatitive code is omitted, but i failed there as some part of code needs to be tracked individually. If I get time, I will revisit this code and then make a routine for call so the less memory in ROM is occupied and more larger programs can be written efficiently. But for now, it is what it is.

-------------------------------------------------ALSO ANY SUGGESTIONS AND IMPROVEMENTS ARE ALWAYS APPRECIATED--------------------------------------------------------------------------
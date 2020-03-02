Name: Johnas Wong
Student ID: 1529241
CMPUT379 Assignment 2: Producer-Consumer Problem (Multithreading)

To run: Compile my program by typing "make" or "make all" for the default compilation.
For the optimization compilation: "make optimization"
For the debugging compilation: "make debugging"

Type in the console "./prodcon a b < inputFileName.txt"
    where "a" is the number of threads, "b" is the optional prodcon.b.log file number,
    and "inputFileName.txt" is the name of the input file.
Or type "./prodcon a b" and manually type in the T<n> and S<n> commands

Notes:
    - With using an inputfile, the user must include ".txt" at the end for my program to run
    - When manually inputting commands, user must enter "CTRL+D" twice for program to recognize 
    end of file. I could not find why
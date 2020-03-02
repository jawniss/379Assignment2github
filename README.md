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
    - When manually inputting commands, user must enter "CTRL+D" twice for program to recognize end of file. I could not find why
    - When manually inputting commands, if something like "T1 T2 T3 S4 T5" followed by a space or enter line or anything, followed by "CTRL+D" program will crash
    - Sometimes when many threads are made, my program gets caught in an infinite loop. Again I'm not sure why, when the consumer threads are waiting they constantly check if number of T<n> commands remaining is > 0, if it isn't it should exit.
    - My producer keeps pushing to the sharedbuffer until it's full at the start; it doesn't push one item then waits. It fills up the buffer then waits when the buffer is filled.
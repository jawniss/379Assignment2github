<pre>
Name: Johnas Wong
Student ID: 1529241
CMPUT379 Assignment 2: Producer-Consumer Problem (Multithreading)

To run: Compile my program by typing "make" or "make all" for the default compilation.
    For the optimization compilation: "make optimization"
    For the debugging compilation: "make debugging"

In the console enter the command:
On MacOS/Linux: "./prodcon &#60;a&#62; &#60;b&#62; < &#60;inputFileName.txt&#62;"
On Windows: "cmd /c '.\prodcon.exe &#60;a&#62; &#60;b&#62; < &#60;inputFileName.txt&#62;'"
    - &#60;a&#62; is the number of threads (mandatory)
    - &#60;b&#62; is the created prodcon.<b>.log file number (optional)
    - &#60;inputFileName.txt&#62; is the name of the input file (optional)

Or enter "./prodcon a b" and manually type in the T&#60;n&#62; and S&#60;n&#62; commands without an input file. Enter "CTRL+D" to terminate the program on MacOS/Linux, currently umable to terminate program and create log file on Windows.

Notes:
    - With using an inputfile, the user must include ".txt" at the end
    - When manually entering commands, user must enter "CTRL+D" twice for program to recognize end of file. Could not find reason as to why this behaviour occurs
    - When manually entering commands, if something like "T1 T2 T3 S4 T5" followed by a space or enter line or anything, followed by "CTRL+D" program will crash
    - Sometimes when many threads are made, my program gets caught in an infinite loop. Again I'm not sure why, when the consumer threads are waiting they constantly check if number of T&#60;n&#62; commands remaining is > 0, if it isn't it should exit.
    - My producer keeps pushing to the sharedbuffer until it's full at the start; it doesn't push one item then waits. It fills up the buffer then waits when the buffer is filled.
</pre>

Chang Liu
CS 111 Lab 0

Included in this lab are multiple files:
1. lab0.c - the source code to be compiled into the executable
2. Makefile - makefile specifying how to build, test, and clean the C code
3. backtrace.png - snapshot of gdb backtrace when the program segfaults
4. breakpoint.png - snapshot of gdb at the point where we dereference a NULL pointer

Testing
My make test target consists of 7 different tests to ensure the functionality of this program:
1. Checks basic functionality given an input file with random text, that an output file is created.
2. Compares the input file to the output file and checks if they are the same.
3. If a nonexistent input file is passed, the program should print to stderr and exit accordingly.
4. Given an existing output file, it will be successfully overwritten.
5. Running the program with the segfault flag on and the catch flag on (in any order), the SIGSEGV signal will be handled and the program will exit accordingly.
6. Given an input file and no output file, the contents of the input file will be redirected to stdout.
7. Given an output file with only read permissions, the program will give an error and exit. 

CFLAGS = -Wall -std=c++11

all: prodcon
optimization: prodconOptimization

# Linker:
prodcon: prodconExaple.o tands.o
	g++ prodconExaple.o tands.o -pthread $(CFLAGS) -o prodcon

prodconOptimization: prodconExaple.o tands.o
	g++ prodconExaple.o tands.o -pthread -O $(CFLAGS) -o prodcon
    
# Compilation commands:
prodconExaple.o: prodconExaple.cpp
	g++ -c prodconExaple.cpp $(CFLAGS) -o prodconExaple.o

tands.o: tands.c
	g++ -c tands.c $(CFLAGS) -o tands.o

clean:
	@rm -rf prodcon prodconExaple.o tands.o prodcon *.log


# Important: make sure you provide a way in your 
# makefile to allow us to compile your program with “-O” 
# (optimization) instead of “-g” (debugging).
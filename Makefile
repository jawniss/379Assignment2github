CFLAGS = -Wall -std=c++11

all: prodcon
optimization: prodconOptimization
debugging: prodconDebug

# Linker:
prodcon: ProducerConsumerProblem.o tands.o
	g++ ProducerConsumerProblem.o tands.o -pthread $(CFLAGS) -o prodcon

prodconOptimization: ProducerConsumerProblem.o tands.o
	g++ ProducerConsumerProblem.o tands.o -pthread -O $(CFLAGS) -o prodcon

prodconDebug: ProducerConsumerProblem.o tands.o
	g++ ProducerConsumerProblem.o tands.o -pthread -g $(CFLAGS) -o prodcon
    
# Compilation commands:
ProducerConsumerProblem.o: ProducerConsumerProblem.cpp
	g++ -c ProducerConsumerProblem.cpp $(CFLAGS) -o ProducerConsumerProblem.o

tands.o: tands.c
	g++ -c tands.c $(CFLAGS) -o tands.o

clean:
	@rm -rf prodcon ProducerConsumerProblem.o tands.o prodcon *.log


# Important: make sure you provide a way in your 
# makefile to allow us to compile your program with “-O” 
# (optimization) instead of “-g” (debugging).
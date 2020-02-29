CFLAGS = -Wall -std=c++11

# Linker:
prodcon: prodconExaple.o tands.o splitTheInput.o
	g++ prodconExaple.o tands.o splitTheInput.o $(CFLAGS) -o prodcon
    
# Compilation commands:
prodconExaple.o: prodconExaple.cpp
	g++ -c prodconExaple.cpp $(CFLAGS) -o prodconExaple.o

tands.o: tands.c
	g++ -c tands.c $(CFLAGS) -o tands.o

splitTheInput.o: splitTheInput.cpp
	g++ -c splitTheInput.cpp $(CFLAGS) -o splitTheInput.o

clean:
	@rm -rf prodcon prodconExaple.o tands.o splitTheInput.o prodcon


# Important: make sure you provide a way in your 
# makefile to allow us to compile your program with “-O” 
# (optimization) instead of “-g” (debugging).
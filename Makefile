# standard Makefile 
CC = g++
CFLAG =--std=c++11 -Wall -lboost_program_options
OUTPUT = mafbfs.out
OMP= -fopenmp
DBG=  -g
all : 
	$(CC) $(CFLAG) $(DBG)  main.cpp -o $(OUTPUT)	
clean : 
	rm -f *~ 
	rm -f *.out 
	rm -f  *.o
	rm -f  *.dat
	rm -f  market1
	rm -f  market2

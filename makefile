CXXFLAGS= -Wall -g
CXX=g++

mainComp: mainComp.o makeASM makeASMFunc makeGANN makeAudioFile
	g++ -g main.o asmfunc.o genann.o AudioFile.o -o out -Wall -lpthread
	rm main.o
	rm asmfunc.o
	rm genann.o
	rm AudioFile.o
	

mainComp.o:
	g++ -std=c++11 -g -c main.cpp -lpthread -I. -Wall -O3

makeASM:
	g++ -g -c main.cpp -I. -S -Wall -lpthread

makeASMFunc:
	gcc -c asmFunctions.s -o asmfunc.o

makeGANN:
	gcc -c genann.c -O3 -o genann.o

makeAudioFile:
	gcc -c AudioFile.cpp -O3 -o AudioFile.o

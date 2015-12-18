
CPPFLAGS=-I/usr/include/eigen3 -std=c++11 -Wall -O3 -ffast-math -g -fopenmp

all: example

example: tracer.o geometry.o lighting.o example.o
	g++ -o $@ tracer.o geometry.o lighting.o example.o -lpng -fopenmp

.PHONY: clean
clean:
	rm *.o


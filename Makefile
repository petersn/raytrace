
CPPFLAGS=-I/usr/include/eigen3 -std=c++11 -Wall -O2 -ffast-math -g

all: example

example: tracer.o geometry.o lighting.o example.o
	g++ -o $@ tracer.o geometry.o lighting.o example.o -lpng

.PHONY: clean
clean:
	rm *.o


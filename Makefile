
CPPFLAGS=-I/usr/include/eigen3 -std=c++11 -Wall -Wextra

all: tracer.o sphere.o

.PHONY: clean
clean:
	rm *.o


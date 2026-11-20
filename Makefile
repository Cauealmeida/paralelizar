CXX ?= g++
FLAGS = -O3 -std=c++11

all: original seq

# Original, sem alterações
original: mandelbrot.cpp
	$(CXX) $(FLAGS) $< -o $@

# Sequencial original + temporizadores
seq: mandelbrot_seq.cpp
	$(CXX) $(FLAGS) $< -o $@

clean:
	rm -f original{,.exe} seq{,.exe} *.o

.PHONY: all clean

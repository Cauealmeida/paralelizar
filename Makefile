CXX ?= g++
FLAGS = -O3 -std=c++11

all: mandelbrot mandelbrot_seq

# Original, sem alterações
mandelbrot: mandelbrot.cpp
	$(CXX) $(FLAGS) $< -o $@

# Sequencial original + temporizadores
mandelbrot_seq: mandelbrot_seq.cpp
	$(CXX) $(FLAGS) $< -o $@

clean:
	rm -f mandelbrot mandelbrot_seq *.o

.PHONY: all clean

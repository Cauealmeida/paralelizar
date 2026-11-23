CXX ?= g++
FLAGS = -O3 -std=c++11

all: original seq static dynamic guided

# Original, sem alterações
original: mandelbrot.cpp
	$(CXX) $(FLAGS) $< -o $@

# Sequencial original + temporizadores
seq: mandelbrot_seq.cpp
	$(CXX) $(FLAGS) $< -o $@

# Paralelização estática
static: mandelbrot_static.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@

# Paralelização dinâmica
dynamic: mandelbrot_dynamic.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@

# Paralelização guiada
guided: mandelbrot_guided.cpp
	$(CXX) $(FLAGS) -fopenmp $< -o $@

clean:
	rm -f original{,.exe} seq{,.exe} static{,.exe} dynamic{,.exe} guided{,.exe} *.o

.PHONY: all clean

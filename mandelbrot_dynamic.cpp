// Versão paralela com OpenMP - estratégia: schedule(dynamic, 1)
// Igual ao mandelbrot_seq.cpp; só o laço de linhas foi paralelizado.
//
// O número de threads é definido pela variável OMP_NUM_THREADS.
//
// Cada execução imprime UMA linha no terminal:
//   threads,leitura_s,calculo_s,escrita_s,total_s
//
// A imagem é gravada em saida_dynamic.txt, para comparar com o original:
//   ./mandelbrot < mandelbrot.in > referencia.txt
//   diff saida_dynamic.txt referencia.txt

#include <complex>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <omp.h>

using namespace std;
using relogio = chrono::steady_clock;

static double segundos(relogio::time_point a, relogio::time_point b) {
	return chrono::duration<double>(b - a).count();
}

int main() {
	// ---------- Leitura ----------
	auto t0 = relogio::now();

	int max_row, max_column, max_n;
	cin >> max_row;
	cin >> max_column;
	cin >> max_n;

	char **mat = (char**)malloc(sizeof(char*)*max_row);

	for (int i=0; i<max_row;i++)
		mat[i]=(char*)malloc(sizeof(char)*max_column);

	//tempo de cálculo é medido a partir daqui (inclui a criação das threads)
	auto t1 = relogio::now();

	#pragma omp parallel for schedule(dynamic, 1)
	for(int r = 0; r < max_row; ++r){
		for(int c = 0; c < max_column; ++c){
			complex<float> z;
			int n = 0;
			while(abs(z) < 2 && ++n < max_n)
				z = pow(z, 2) + decltype(z)(
					(float)c * 2 / max_column - 1.5,
					(float)r * 2 / max_row - 1
				);
			mat[r][c]=(n == max_n ? '#' : '.');
		}
	}

	auto t2 = relogio::now();

	ofstream saida("saida_dynamic.txt");
	for(int r = 0; r < max_row; ++r){
		for(int c = 0; c < max_column; ++c)
			saida << mat[r][c];
		saida << '\n';
	}
	saida.close();

	auto t3 = relogio::now();

	cout.setf(ios::fixed);
	cout.precision(6);
	cout << omp_get_max_threads() << ','
	     << segundos(t0, t1) << ',' << segundos(t1, t2) << ','
	     << segundos(t2, t3) << ',' << segundos(t0, t3) << '\n';

	for (int i=0; i<max_row;i++) free(mat[i]);
	free(mat);
}

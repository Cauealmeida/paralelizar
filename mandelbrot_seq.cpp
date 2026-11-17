// Versão sequencial ORIGINAL com medição de tempo.
// O algoritmo é idêntico ao mandelbrot.cpp; só foram adicionados temporizadores.
//
// Uso (5 execuções independentes, cada uma é um processo novo):
//   for i in 1 2 3 4 5; do ./mandelbrot_seq < mandelbrot.in; done
//
// Cada execução imprime UMA linha no terminal:
//   leitura_s,calculo_s,escrita_s,total_s
//
// A imagem é gravada em saida_seq.txt, para comparar com o original:
//   ./mandelbrot < mandelbrot.in > referencia.txt
//   diff saida_seq.txt referencia.txt

#include <complex>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cstdlib>

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

	//tempo de cálculo é medido a partir daqui
	auto t1 = relogio::now();

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

	ofstream saida("saida_seq.txt");
	for(int r = 0; r < max_row; ++r){
		for(int c = 0; c < max_column; ++c)
			saida << mat[r][c];
		saida << '\n';
	}
	saida.close();

	auto t3 = relogio::now();

	cout.setf(ios::fixed);
	cout.precision(6);
	cout << segundos(t0, t1) << ',' << segundos(t1, t2) << ','
	     << segundos(t2, t3) << ',' << segundos(t0, t3) << '\n';

	for (int i=0; i<max_row;i++) free(mat[i]);
	free(mat);
}

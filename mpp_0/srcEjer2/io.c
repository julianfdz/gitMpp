#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define min 0
#define max 10

double *generar_matriz_distancias(int n)
{
	double f;
	double *d = (double *) malloc(n * n *sizeof(double));
	
	srand(time(NULL) + getpid());
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {
			f = (double) rand() / ((double) RAND_MAX + 1);
			// De esta forma tenemos una matriz triangular
			// (menos eficiente en cuanto a uso de memoria)
			*(d + (i*n + j)) = (j > i) ? (min + f*(max - min)) : 0.0;
		}
	}

	return d;
}

void print_matrix(double *d, int n)
{
	for(int i = 0; i <= n*n; i++) {
		if ((i % n) != 0) { printf("%.2lf ", *(d + i)); }
		else { printf("\n%.2lf ", *(d + i)); }
	}
	printf("\n\n");
}

void print_solution(int n, int m, const int *solucion, double valor)
{
	printf("\nSolution: ");
	for(int i = 0; i <= m; i++) { printf("%d ", solucion[i]); }
	printf("\nDistance: %.0lf\n", valor);
}

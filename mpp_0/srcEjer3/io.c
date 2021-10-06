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
	int aux=0;
	for(int i=0; i<n;i++)
		aux+=i;
	double *d = (double *) malloc(aux *sizeof(double));
	
	srand(time(NULL) + getpid());
	for(int i = 0; i < aux; i++) {
			f = (double) rand() / ((double) RAND_MAX + 1);
			// De esta forma tenemos una matriz triangular
			// (menos eficiente en cuanto a uso de memoria)
			d[i] = (min + f*(max - min));
		
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
	for(int i = 0; i < m; i++) { printf("%d ", solucion[i]); }
	printf("\nDistance: %.0lf\n", valor);
}

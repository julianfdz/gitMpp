#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../include/mdp.h"

#define MUTATION_RATE 0.15
#define PRINT 1
#define PORCENTDIFF 1.02

int aleatorio(int n) {
	return (rand() % n);  // genera un numero aleatorio entre 0 y n-1
}

int find_element(int *array, int end, int element)
{
	int i=0;
	int found=0;

	// comprueba que un elemento no está incluido en el individuo (el cual no admite enteros repetidos)
	while((i < end) && ! found) {

		if(array[i] == element) {
			found = 1;
		}
		i++;
	}

	return found;
}

int find_index_element(int *array, int end, int element)
{
	int i=0;
	int cont=0;
	// comprueba que un elemento no está incluido en el individuo (el cual no admite enteros repetidos)
	while((i < end)) {

		if(array[i] == element) {
			cont++;
			
		}
		i++;
	}

	return cont;
}

int *crear_individuo(int n, int m)
{
	int i=0, value;
	int *individuo = (int *) malloc(m * sizeof(int));
	
	// inicializa array de elementos
	memset(individuo, -1, m * sizeof(int));
	
	while(i < m) {
		value = aleatorio(n);
		// si el nuevo elemento no está en el array...
		if(!find_element(individuo, i, value)) {
			individuo[i] = value;  // lo incluimos
			i++;
		}
	}
	return individuo;
}

int comp_array_int(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

int comp_fitness(const void *a, const void *b) {
	/* qsort pasa un puntero al elemento que está ordenando */
	return (*(Individuo **)a)->fitness - (*(Individuo **)b)->fitness;
}

double aplicar_mh(const double *d, int n, int m, int n_gen, int tam_pob, int *sol)
{
/*
	for(int i = 0; i <= n*n; i++) {
		if ((i % n) != 0) { printf("%.2lf ", *(d + i)); }
		else { printf("\n%.2lf ", *(d + i)); }
	}
	printf("\n\n");
*/
	int i, g=0, mutation_start;

	
	// crea poblacion inicial (array de individuos)
	Individuo **poblacion = (Individuo **) malloc(tam_pob * sizeof(Individuo *));
	assert(poblacion);

	// crea cada individuo (array de enteros aleatorios)
	

	for(i = 0; i < tam_pob; i++) {
		poblacion[i] = (Individuo *) malloc(sizeof(Individuo));
		poblacion[i]->array_int = crear_individuo(n, m);
		/*
		for(int j=0;j<m;j++)
		{
			printf("%d ", poblacion[i]->array_int[j]);
		}*/

		// calcula el fitness del individuo
		
		fitness(d, poblacion[i], n, m);
	}
	
	// ordena individuos segun la funcion de bondad (mayor "fitness" --> mas aptos)
	qsort(poblacion, tam_pob, sizeof(Individuo *), comp_fitness);

	// evoluciona la poblacion durante un numero de generaciones
	int flag=1;
	float fitnessAnterior, fitnessActual;
	fitnessActual = -poblacion[0]->fitness;
	fitnessAnterior=-poblacion[0]->fitness;
	while(flag)
	{

		//Variable con el numero de generaciones
		g++;
		// los hijos de los ascendientes mas aptos sustituyen a la ultima mitad de los individuos menos aptos
		for(i = 0; i < (tam_pob/2) - 1; i += 2) {
			cruzar(poblacion[i], poblacion[i+1], poblacion[tam_pob/2 + i], poblacion[tam_pob/2 + i + 1], n, m);
	
		}

		// inicia la mutacion a partir de 1/4 de la poblacion
		mutation_start = tam_pob/4;
		
		// muta 3/4 partes de la poblacion
		for(i = mutation_start; i < tam_pob; i++) {
			mutar(poblacion[i], n, m);
		}

		// recalcula el fitness del individuo

		for(i = 0; i < tam_pob; i++) {
			fitness(d, poblacion[i], n, m);
		}

		// ordena individuos segun la funcion de bondad (mayor "fitness" --> mas aptos)
		qsort(poblacion, tam_pob, sizeof(Individuo *), comp_fitness);
		if (PRINT) {
			printf("Generacion %d - ", g);
			printf("Fitness = %.0lf\n", -(poblacion[0]->fitness));
		}
		//fitnessActual es el positivo del mejor fitness de la poblacion
		fitnessActual = -poblacion[0]->fitness;
		printf("%f %f\n", poblacion[0]->fitness, fitnessAnterior);
		if(fitnessActual < (fitnessAnterior*PORCENTDIFF)){
		flag=0;
		}

		//fitness anterior ahora tiene el valor del mejor individuo para la siguiente iteracion
		fitnessAnterior=-poblacion[0]->fitness;

	}
	

	
	// ordena el array solucion

	qsort(poblacion[0]->array_int, m, sizeof(int), comp_array_int);
	memmove(sol, poblacion[0]->array_int, m*sizeof(int));

	double value = -(poblacion[0]->fitness);
	// se libera la memoria reservada
	for(i = 0; i < tam_pob; i++) {
		free(poblacion[i]->array_int);
		free(poblacion[i]);
	}
	free(poblacion);
	
	// devuelve el valor obtenido para el fitness
	return value;
}

void cruzar(Individuo *padre1, Individuo *padre2, Individuo *hijo1, Individuo *hijo2, int n, int m)
{
    // Elegir un "punto" de corte aleatorio a partir del que se realiza el intercambio de los genes
    // Los primeros genes del padre1 van al hijo1. Idem para el padre2 e hijo2.
    // Y los restantes son del otro padre, respectivamente.
    // Factibilizar: eliminar posibles repetidos de ambos hijos
    // Si encuentro alguno repetido en el hijo1, lo cambio por otro que no este en el conjunto

    //asignamos el punto de corte de manera aleatoria
    int pc = aleatorio(m - 1) + 1;
    //repartimos los genes de los padresS
    for( int i = 0; i <= pc; i++){
        (hijo1)->array_int[i] = (padre1)->array_int[i];
        (hijo2)->array_int[i] = (padre2)->array_int[i];
    }
    for( int i = pc +1; i < m; i++){
        (hijo1)->array_int[i] = (padre2)->array_int[i];
        (hijo2)->array_int[i] = (padre1)->array_int[i];
    }
    //comprobamos que no haya repetidos
    for( int i = 0; i < m; i++)
    {
	 while( find_index_element((hijo1)->array_int, m, (hijo1)->array_int[i]) != 1)
	 { 
            	(hijo1)->array_int[i] = aleatorio(n);
        }
    }
    for( int i = 0; i < m; i++){
	 while( find_index_element((hijo2)->array_int, m, (hijo2)->array_int[i]) != 1)
	 	{ 
		(hijo2)->array_int[i] = aleatorio(n);
            	}      
        }
}

//d=Array unidimensional de elementos, i=elemento que se quiere sumar, j=el otro elemento que se quiere sumar, n =tama;o de l array
//Calcular en el fitness las posiciones 
//d=arrayDeDistancias, i=coordenada x, j=coordenada y, n=tam;o array
double distancia_ij(const double *d, int i, int j, int n)
{
	if(i==0){return d[j-1];}
	int aux = (i*(i+1))/2;

	return d[aux+j-1];
}

void fitness(const double *d, Individuo *individuo, int n, int m)
{	
	float fitness = 0;
	individuo->fitness = 0;
	for(int i=0;i<m-1;i++)
	{
		for(int j=i+1;j<m;j++)
		{
			fitness += distancia_ij(d, individuo->array_int[i], individuo->array_int[j], n);
		}
	}
	individuo->fitness = -fitness;
	// Determina la calidad del individuo calculando la suma de la distancia entre cada par de enteros
}
void mutar(Individuo *actual, int n, int m)
{
    // Decidir cuantos elementos mutar:
    // Si el valor es demasiado pequeño la convergencia es muy pequeña y si es demasiado alto diverge

    // Cambia el valor de algunos elementos de array_int de forma aleatoria
    // teniendo en cuenta que no puede haber elementos repetidos.
    int nElem = m*MUTATION_RATE;
	for(int i=0;i<nElem;i++){
        int pos = aleatorio(m);
        int valor = aleatorio(n);
        while( find_element((actual)->array_int, m, valor)){ //como maximo un gen puede tener un repetido ya que los padres no tienen repetidos
            valor = aleatorio(n);
        }
        (actual)->array_int[pos] = valor;
    }
}

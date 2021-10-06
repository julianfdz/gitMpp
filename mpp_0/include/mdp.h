#ifndef _MDP
#define _MDP
	
	typedef struct {
		int *array_int;
		double fitness;
	} Individuo;
	
	void cruzar(Individuo *, Individuo *, Individuo *, Individuo *, int, int);
	void fitness(const double *, Individuo *,int, int);
	void mutar(Individuo *, int, int);
#endif

/* main.c
 * Authors : DELAVERNHE Florian, LEGRU Guillaume
 * Date : 05/01/2016
 */


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/resource.h>

#define NBFOURMI 20;

struct timeval start_utime, stop_utime;

void crono_start()
{
	struct rusage rusage;
	
	getrusage(RUSAGE_SELF, &rusage);
	start_utime = rusage.ru_utime;
}

void crono_stop()
{
	struct rusage rusage;
	
	getrusage(RUSAGE_SELF, &rusage);
	stop_utime = rusage.ru_utime;
}

double crono_ms()
{
	return (stop_utime.tv_sec - start_utime.tv_sec) * 1000 +
    (stop_utime.tv_usec - start_utime.tv_usec) / 1000 ;
}

typedef struct
{
	int nbctr;
	int nbvar;
	int** matrix;
	int* coef;
} data;

void display (const int* sol, const int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d ", sol[i]);
	}
	printf("\n");
}

double cout(int* sol, data* dat)
{
	int i;
	double val = 0;
	for (i = 0; i < dat->nbvar; ++i)
	{
		val += sol[i] *(double) dat->coef[i];
		
	}
	return val;
}

int readfile(data* dat, char* datafile)
{
	FILE *fin;
	int val; 
	int i,j,k;
	int ctrSize;
	
	fin = fopen(datafile, "r");

	//nbctr nbvar
	fscanf(fin, "%d", &val);
	dat->nbctr = val;
	fscanf(fin, "%d", &val);
	dat->nbvar = val;
	
	dat->coef = (int *) malloc (dat->nbvar * sizeof(int));
	for (i = 0; i < dat->nbvar; ++i)
	{
		fscanf(fin, "%d", &val);
		dat->coef[i] = val;
	}
	
	dat->matrix = (int **) malloc (dat->nbctr * sizeof(int*));
	for (i = 0; i <= dat->nbctr; ++i)
	{
		dat->matrix[i] = (int *) malloc (dat->nbvar * sizeof(int));
		for (k = 0; k < dat->nbvar; ++k)
		{
			dat->matrix[i][k] = 0;
		}
		
		fscanf(fin, "%d", &ctrSize);
		for (j=0; j < ctrSize; ++j)
		{
			fscanf(fin, "%d", &val);
			dat->matrix[i][val] = 1;

						
		}
	}
	return 1; 
}

int main (int argc, char** argv)
{
	data dat;
	int i; 
	readfile(&dat, argv[1]);

	int** fourmis = (int **) malloc(sizeof(int*) * NBFOURMI);
	for (i = 0; i < NBFOURMI; i++)
	{
		fourmis[i] = (int *) malloc(sizeof(int) * dat.nbvar);
	}

	double[dat.nbvar] prob0;
	for (i = 0; i < dat.nbvar; i++)
	{
		prob0[i] = 1; 
	}

	double[dat.nbvar] prob1;
	for (i = 0; i < dat.nbvar; i++)
	{
		prob1[i] = 1; 
	}

	double valOpti = 0;

	int ITERMAX = atoi(argv[2]);
	int iter = 1;
	while(iter < ITERMAX)
	{
		for (j = 0; j < NBFOURMI; j++)
		{
			for (i = 0; i < dat.nbvar; i++)
			{
				fourmis[j][i]
			}

			for (i = 0; i < dat.nbvar; i++)
			{
				
			}
		}
	}
}

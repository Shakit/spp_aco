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

// data structure
typedef struct
{
	int nbctr;
	int nbvar;
	int** matrix;
	int* coef;
} data;

// array of int display procedure
void display (const int* sol, const int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d ", sol[i]);
	}
	printf("\n");
}

// array of double display procedure
void displayd (const double* sol, const int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%f ", sol[i]);
	}
	printf("\n");
}

// profit computation
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

// file reading function
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

//----------------------------------------------------------------------------------------------------
// MAIN FUNCTION
int main (int argc, char** argv)
{
	srand((unsigned int) time(0));
	data dat;
	int i,j,k,l;
	int ITERMAX;
	int NBFOURMI;
	double EVAPORATION;
	double random;

	double temps;

	// parameters checking
	if (argc != 5)
	{
		printf("error : usage : %s  <datafile> <nbiteration> <nbants> <evaporation>\n", argv[0]);
		exit(1);
	}
	printf("datafile : %s\n", argv[1]);
	printf("nbIterations : %s\n", argv[2]);
	printf("nbAnts : %s\n", argv[3]);
	printf("evaporation : %s\n", argv[4]);

	ITERMAX = atoi(argv[2]);
	NBFOURMI = atoi(argv[3]);
	EVAPORATION = atof(argv[4]);

	//file reading
	readfile(&dat, argv[1]);
	crono_start();

	//initialisations
	int* fourmis[NBFOURMI];
	for (i = 0; i < NBFOURMI; i++)
	{
		fourmis[i] = (int *) malloc(sizeof(int) * dat.nbvar);
	}
	double coutF[NBFOURMI];
	for (i = 0; i < NBFOURMI; i++)
	{
		coutF[i] = 0;
	}
	double prob0[dat.nbvar];
	for (i = 0; i < dat.nbvar; i++)
	{
		prob0[i] = NBFOURMI; 
	}
	double prob1[dat.nbvar];
	for (i = 0; i < dat.nbvar; i++)
	{
		prob1[i] = NBFOURMI; 
	}
	double valOpti = 0;
	int solOpti[dat.nbvar];
	for (i = 0; i < dat.nbvar; i++)
	{
		solOpti[i] = 0;
	}

	/* repeat
	 *    - ants travelling
	 *    - evaporation
	 *    - evaluation
	 *    - pheromone deposit
	 * until ( iter == ITERMAX )	  
	 */
	int iter = 0;
	while(iter < ITERMAX)
	{
		for (j = 0; j < NBFOURMI; j++)
		{
			for (i = 0; i < dat.nbvar; i++)
			{
				fourmis[j][i] = -1;
			}

			for (i = 0; i < dat.nbvar; i++)
			{
				if (fourmis[j][i] == -1)
				{
					random = (rand() % (NBFOURMI * ITERMAX +1)) / (double) (NBFOURMI * ITERMAX + 1) * (prob0[i] + prob1[i]);

					if (random < prob0[i])
					{
						fourmis[j][i] = 0;
					}
					else
					{
						fourmis[j][i] = 1;	
						for (k = 0; k < dat.nbctr; k++)
						{
							if (dat.matrix[k][i] == 1)
							{
								for (l = i+1; l < dat.nbvar; l++)
								{
									if (dat.matrix[k][l] == 1 ) fourmis[j][l] = 0;
								}
							} 
						} 
					} 
 				} 
			} // end for i (nbvar) 
		} // end for j (fourmis) 

		// evaporation
		for (i = 0; i < dat.nbvar; i++)
		{
			prob0[i] *= EVAPORATION;
			prob1[i] *= EVAPORATION;
		}

		// solutions evaluation 
		for (i = 0; i < NBFOURMI; i++)
		{
			coutF[i] = cout(fourmis[i], &dat);
		}
		double min = coutF[0];
		double max = coutF[0];
		int pos = 0;
		for (i = 1; i < NBFOURMI; i++)
		{
			if(min >coutF[i])
			{
				min = coutF[i];
			}
			if ( max < coutF[i])
			{
				max = coutF[i];
				pos = i;
			}
		}		
		if(max > valOpti)
		{
			valOpti = max;
			for (i = 0; i < dat.nbvar; i++)
			{
				solOpti[i] = fourmis[pos][i];
			}
			printf("\nBetter solution found ! \n");
			printf("%f\n",valOpti);
			display(solOpti, dat.nbvar);
			printf("--------------------------------------------------\n");
		}

		// pheromone deposit
		if(min != max)
		{	
			for (i = 0; i < dat.nbvar; i++)
			{
				for (j = 0; j < NBFOURMI; j++)
				{
					if(fourmis[j][i] = 1)
					{
						prob1[i] += (coutF[j]-min) / (max - min);
					}
					else
					{
						prob0[i] += (coutF[j]-min) / (max - min);
					}

				}

			}
		}
		iter++;
	}// end while
	
	crono_stop();
	temps = crono_ms()/1000,0;

	// Solution display
	printf("\n\nBest solution : \n");
	printf("%f\n",valOpti);
	display(solOpti, dat.nbvar);
	printf("Computation time : %f\n", temps);
	
	for (i = 0; i < dat.nbctr; ++i)
	{
		free(dat.matrix[i]);
	}
	free(dat.matrix);
	free(dat.coef);
	for (i = 0; i < NBFOURMI; i++)
	{
		free(fourmis[i]);
	}

}

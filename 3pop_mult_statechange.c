#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "3pop_mult_statechange.h"


int *get_neighbours(int index, int number_row, int number_column);

int main()
{
	// srand(time(0));
	srand(10);
	// srand(9);
	// srand(11);
	int number_row = 20; // must be EVEN
	int number_column = 10;
	int init_num_onsite = 20;
	float diffusion_D = 1.5;
	float bias = 0.9;

	int total_on_LATTICE = 0;

	int rand_site = 0;
	int rand_agent = 0;

	FILE *POP_0 = fopen("MULT_0.txt","w");
	if (POP_0 == NULL)
	{
		perror("\nERR OPEN POP_0\n");
		return 1;
	}
	FILE *POP_1 = fopen("MULT_1.txt","w");
	if (POP_0 == NULL)
	{
		perror("\nERR OPEN POP_1\n");
		return 1;
	}
	FILE *POP_2 = fopen("MULT_2.txt","w");
	if (POP_0 == NULL)
	{
		perror("\nERR OPEN POP_2\n");
		return 1;
	}
	FILE *POP_tot = fopen("MULT_total.txt","w");
	if (POP_tot == NULL)
	{
		perror("\nERR OPEN POP_2\n");
		return 1;
	}

	// CREATE LATTICE
	struct site* lattice = malloc(number_column * number_row * sizeof(struct site));
	if (lattice == NULL)
	{
		printf("GRID MEM ALLOC FAIL\n");
		return 1;
	}
	for(int c = 0; c < number_row*number_column; c++)
	{
		// int num_agents_onsite = rand()%(1+init_num_onsite);
		int num_agents_onsite = init_num_onsite;
		total_on_LATTICE = total_on_LATTICE + num_agents_onsite;
		create_site(&lattice[c], c, num_agents_onsite);
		printf("\n\nSITE %d:", c);
		print_site(&lattice[c]);
		fprintf(POP_0, "%d,", lattice[c].population_0);
		fprintf(POP_1, "%d,", lattice[c].population_1);
		fprintf(POP_2, "%d,", lattice[c].population_2);
		fprintf(POP_tot, "%d,", lattice[c].total_pop);
	}
	printf("\nTOTAL ON LATTICE: %d", total_on_LATTICE);

	for (int i = 0; i<60000; i++)
	{
		total_on_LATTICE = 0;
		// SELECT SITE THEN AGENT TO HOP
		rand_site = rand()%(number_column * number_row);
		while (lattice[rand_site].total_pop == 0)
		{
			rand_site = rand()%(number_column * number_row);
			printf("\nEMPTY SITE SELECTED, CHOOSE OTHER SITE: %d",rand_site);
		}
		printf("\nRAND SELECT site: %d.",rand_site);

		// BIG FUNCTION
		hop_from(rand_site, lattice, diffusion_D, bias, number_row, number_column);
		// BIG FUNCTION
		printf("\n\n");
		for (int i = 0; i<number_column*number_row; i++)
		{
			printf("num on site %d: %d\n", i, lattice[i].total_pop);
			total_on_LATTICE = total_on_LATTICE + lattice[i].total_pop;
		}
		printf("\nTOTAL ON LATTICE: %d", total_on_LATTICE);

		fprintf(POP_0, "\n");
		fprintf(POP_1, "\n");
		fprintf(POP_2, "\n");
		fprintf(POP_tot, "\n");
		for (int c = 0; c < number_row*number_column; c++)
		{
			fprintf(POP_0, "%d,", lattice[c].population_0);
			fprintf(POP_1, "%d,", lattice[c].population_1);
			fprintf(POP_2, "%d,", lattice[c].population_2);
			fprintf(POP_tot, "%d,", lattice[c].total_pop);
		}
		printf("\n\nITERATION: %d\n",i);
	}



	for(int i = 0; i<number_row*number_column; i++)
	{
		free_site(&lattice[i]);
		// printf("\nSite %d freed.",i);
	}
	free(lattice);
	printf("\n\nlattice freed.\n");
	fclose(POP_0);
	fclose(POP_1);
	fclose(POP_2);
	fclose(POP_tot);
	
}

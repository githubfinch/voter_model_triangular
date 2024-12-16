// RPSRPSRPSRPSRPSRPS
// RPSRPSRPSRPSRPSRPS
// RPSRPSRPSRPSRPSRPS
// RPSRPSRPSRPSRPSRPS
// RPSRPSRPSRPSRPSRPS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "exc_links.h"

int* get_neighbours(int index, int number_row, int number_column);

int main()
{
	// srand(time(0));
	srand(10);
	int number_row = 80; // must be EVEN
	int number_column = 40; 

	int count_rival_pairs = 0;
	int first_rivals_count = 0; // ALLOCATE MEMORY

	int rand_select = 0;

	int* rival_pairs; // allocated AFTER COUNTING rival pairs
	int* neighbour_c;
	int* all_states;
	all_states = malloc(number_row*number_column * sizeof(int));

	int R_count = 0;
	int P_count = 0;
	int S_count = 0;

// WRITE TO FILES
	FILE *file = fopen("states_RPS.txt","w");
	if (file == NULL)
	{
		perror("ERR OPEN FILE\n");
		return 1;
	}
	FILE *three_pop_FILE = fopen("3_pop_RPS.txt","w");
	if (three_pop_FILE == NULL)
	{
		perror("ERR 3POP FILE\n");
		return 1;
	}

// RAND ASSIGN STATES TO LATTICE
	for(int c = 0; c < number_row*number_column; c++)
	{
		all_states[c] = rand() % 3; // POPULATE states array with {0,1,2}
		printf("%d,", all_states[c]);
		fprintf(file, "%d", all_states[c]);
		if (all_states[c] == 0)
		{
			R_count++;
		}
		else if (all_states[c] == 1)
		{
			P_count++;
		}
		else if (all_states[c] == 2)
		{
			S_count++;
		}
	}
	fprintf(file,"\n");
	fprintf(three_pop_FILE, "%d,%d,%d\n", R_count, P_count, S_count);
	R_count = 0; S_count = 0; P_count = 0;
	for(int c = 0; c < number_row*number_column; c++) // c is the linear index
	{
		neighbour_c = get_neighbours(c, number_row, number_column);
		printf("\n");
		printf("%d", neighbour_c[0]);
		printf(", ");
		printf("%d", neighbour_c[1]);
		printf(", ");
		printf("%d", neighbour_c[2]);
		// COUNTING RIVALS
		for(int n = 0; n < 3; n++) // iterate through single neighbourhood
		{
			if (all_states[c] != all_states[neighbour_c[n]]) // if target states is different to neighbours
			{
				first_rivals_count++; 
			}
		}
		free(neighbour_c);
	}
	// rival_pairs IS AN ARRAY
	rival_pairs = malloc(first_rivals_count * sizeof(int));
	for(int c = 0; c < number_row*number_column; c++)
	{
		neighbour_c = get_neighbours(c, number_row, number_column);
		for(int n = 0; n < 3; n++) // iterate through single neighbourhood
		{
			if (all_states[c] != all_states[neighbour_c[n]]) // if target states is different to neighbours
			{ // all pairs appear twice
				rival_pairs[count_rival_pairs] = c; // every even element (0,2,4,.) is the first of a rival pair
				rival_pairs[count_rival_pairs+1] = neighbour_c[n]; // every odd element (1,3,5,.) is the second
				count_rival_pairs += 2; // indices of pairs: (0,1), (2,3), (4,5),...
			}
		}
	}
// CREATE LINKED LIST FROM rival_pairs, just a list with a single chain
	struct Node* linked_rivals = createList(rival_pairs, count_rival_pairs);
	printf("\nfirst_rivals_count: %d", first_rivals_count);
	printf("\nnumber of rivals: %d", count_rival_pairs);
	printf("\nnumber of RIVALS: %d", list_length(linked_rivals));
// DISPLAY LATTICE
	printf("\nRIVAL LIST: ");
	for(int c = 0; c < count_rival_pairs; c++)
	{
		printf("%d,", rival_pairs[c]);
	}
	printf("\n");
// LINK UP SAME VALUED ENTRIES, for specific valued entry see later
	link_occurs(linked_rivals);
	print_list(linked_rivals);

	int iterator = 0;
	// while (list_length(linked_rivals) != 0)
	for (int i = 0; i<100000; i++)
	{
		rand_select = rand() % count_rival_pairs;
		if (rand_select%2 == 0)
		{
			printf("\nRAND SELECT (%d,%d)", rand_select, rand_select+1);
		}
		else if (rand_select%2 == 1)
		{
			printf("\nRAND SELECT (%d,%d)", rand_select, rand_select-1);
		}

	// INIT
		int select_index = get_list_data(linked_rivals,rand_select);
		int conform_index;
	// CHOSEN ONE IS CONFORMED (update all_states)
		if ((rand_select) % 2 == 0)
		{
			int select_index_NEXT = get_list_data(linked_rivals,rand_select+1);
			printf("\nindices of pair: %d,%d.",select_index,select_index_NEXT);
			printf("\nOLD STATE OF SELECTED ENTRY: %d,%d.", all_states[select_index],all_states[select_index_NEXT]);
			conform_index = rps_game(all_states, number_column*number_row, select_index, select_index_NEXT);
			printf("\nNEW STATE OF SELECTED ENTRY: %d,%d.\n", all_states[select_index],all_states[conform_index]);
		}
		else if ((rand_select) % 2 == 1)
		{
			int select_index_PREV = get_list_data(linked_rivals,rand_select-1);
			printf("\nindices of pair: %d,%d.",select_index,select_index_PREV);
			printf("\nOLD STATEs OF SELECTED ENTRYies: %d,%d.", all_states[select_index],all_states[select_index_PREV]);
			conform_index = rps_game(all_states, number_column*number_row, select_index, select_index_PREV);
			printf("\nNEW STATEs OF SELECTED ENTRYies: %d,%d.\n", all_states[select_index],all_states[conform_index]);
		}

	// SAVE DATA
		for (size_t i = 0; i<number_row*number_column; i++)
		{
			fprintf(file, "%d", all_states[i]);
			if (all_states[i] == 0)
			{
				R_count++;
			}
			else if (all_states[i] == 1)
			{
				P_count++;
			}
			else if (all_states[i] == 2)
			{
				S_count++;
			}
		}
		fprintf(three_pop_FILE, "%d,%d,%d\n", R_count, P_count, S_count);
		fprintf(file, "\n");
		R_count = 0; S_count = 0; P_count = 0;

	// ISOLATE LIST OF THE SAME VALUE (INDEX IN LATTICE)
		int count;
		int* chain_of_indices = isolate_list(linked_rivals, conform_index, &count);

	// REMOVE OLD NEIGHBOURHOOD OF CONFORMED ENTRY
		printf("TARGET HEAD: %d, CHAIN LENGTH: %d\n", conform_index, count);
		printf("ISOLATED CHAIN: ");
		for (int c=0 ; c<count; c++)
		{
			printf("%d,", chain_of_indices[c]);
		}
		printf("\nISOLATED CHAIN: ");
	// REMOVE OLD NEIGHBOURHOOD
		for (int c = 0; c<count ; c++)
		{ // e.g. (...,5,4,8,9,...)
			printf("%d,", chain_of_indices[c]);
			if (c%2 == 0 && chain_of_indices[c]%2 == 1 && chain_of_indices[c]==chain_of_indices[c+1]+1)
			{ // 5, first of pair in chain, second of pair in rivals_list, 5=4+1
				remove_node(&linked_rivals, chain_of_indices[c] - c - 1);
			}
			else if (c%2 == 1 && chain_of_indices[c]%2 == 0 && chain_of_indices[c]==chain_of_indices[c-1]-1)
			{ // 4, second of pair in chain, first of pair in rivals_list, 4=5-1
				remove_node(&linked_rivals, chain_of_indices[c] - c + 1);
			}
			else
			{
				remove_node(&linked_rivals, chain_of_indices[c] - c);
			}
		}
	// UPDATE NEIGHBOURHOOD
		int* temp_neighbourhood = get_neighbours(conform_index, number_row, number_column);
		int* temp_rival_list = malloc(6 * sizeof(int));
		int temp_rival_count = 0;
		for(int n = 0; n < 3; n++) // iterate through single neighbourhood
		{
			if (all_states[temp_neighbourhood[n]] != all_states[conform_index])
			{
				temp_rival_list[temp_rival_count] = conform_index;
				temp_rival_list[temp_rival_count +1] = temp_neighbourhood[n]; 
				temp_rival_count += 2;
			}
		}
		printf("\nNEW RIVALS IN NEIGHBOURHOOD: %d", temp_rival_count/2);
		printf("\nNEW RIVAL LIST: \n");
	// APPEND
		for(int c = 0; c < temp_rival_count; c++)
		{
			printf("%d,", temp_rival_list[c]);
			append_node(&linked_rivals, temp_rival_list[c]);
		}

		link_occurs(linked_rivals);
		printf("\nCURRENT LATTICE: ");
		for(int c = 0; c < number_row*number_column; c++)
		{
			printf("%d,", all_states[c]);
		}
		printf("\n");

		printf("\nRIVAL LIST:\n");
		print_list(linked_rivals);

		iterator++;
		printf("!!!%d!!!",iterator);

		count_rival_pairs = list_length(linked_rivals);
		free(temp_neighbourhood);
		free(temp_rival_list);
		free(chain_of_indices);
	}

	free_list(linked_rivals);
	free(all_states);
	free(rival_pairs);
	printf("\n\nall_states FREED.\n");
	fclose(file);
	fclose(three_pop_FILE);
	
}


/*
Lattice structure:
. . . . .
 . . . . .
  . . . . .
   . . . . .
    . . . . . ...
(Shown here are the vertices of the lattice)
*/
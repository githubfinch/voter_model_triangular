# include <stdio.h>
# include <stdlib.h>


struct agent
{
	int state;
	int location;
};

struct site
{
	int population_0;
	int population_1;
	int population_2;
	struct agent *onsite_agents;
	int total_pop;
	int overall_state;
};

struct agent create_agent(int index)
{
	struct agent new_agent;
	new_agent.state = rand()%3;
	new_agent.location = index;
	return new_agent;
}

int find_max(int a, int b, int c)
{
	int maximum;
	int temp;
	if (a>b)
	{
		temp = a;
	}
	else
	{
		temp = b;
	}
	if (c>temp)
	{
		maximum = c;
	}
	else
	{
		maximum = temp;
	}

	return maximum;
}

int effective_state(int pop0_size, int pop1_size, int pop2_size)
{	// not the safest definition at all...
	// rethink value assigned to states ({0,1,2} or {-1,0,1} or something else)
	int state;
	if (find_max(pop0_size,pop1_size,pop2_size) == pop0_size)
	{
		state = 0;
	}
	else if (find_max(pop0_size,pop1_size,pop2_size) == pop1_size)
	{
		state = 1;
	}
	else if (find_max(pop0_size,pop1_size,pop2_size) == pop2_size)
	{
		state = 2;
	}
}

float rand_float_inrange(float start, float stop)
{
	float rand_num;
	float scale = rand()/(float)RAND_MAX;
	rand_num = start + scale*(stop-start);
	return rand_num;
}

void create_site(struct site* s, int tot_num_agent)
{
	s->population_0 = 0;
	s->population_1 = 0;
	s->population_2 = 0;
	s->total_pop = tot_num_agent;
	s->onsite_agents = (struct agent*)malloc(tot_num_agent * sizeof(struct agent));
	s->overall_state = NULL;
	for (int i = 0; i < tot_num_agent; i++)
	{
		s->onsite_agents[i] = create_agent(i);
		if (s->onsite_agents[i].state == 0)
		{
			s->population_0++;
		}
		else if (s->onsite_agents[i].state == 1)
		{
			s->population_1++;
		}
		else if (s->onsite_agents[i].state == 2)
		{
			s->population_2++;
		}
	}
	s->overall_state = effective_state(s->population_0,s->population_1,s->population_2);
}

void hop_from(int old_location, struct site* lattice, int carry_capacity, float diffusion_D, float bias, int num_row, int num_col)
{
	int chosen_agent = rand() % lattice[old_location].total_pop;
	struct agent *moving_agent = &lattice[old_location].onsite_agents[chosen_agent];
	int state_of_chosen = moving_agent->state;
	int new_location;
	float diffusion_rate;
	int *old_neighbours = get_neighbours(old_location,num_row,num_col);
	int *available_neighbours;
	for (int i = 0; i<3; i++)
	{
		int neighbour_i = old_neighbours[i];
		if (lattice[neighbour_i].total_pop < carry_capacity)
		{
			available_neighbours[i] = neighbour_i;
		}
	}
	if (available_neighbours == NULL)
	{
		printf("\n\nALL NEIGHBOURS FULL, STAY WHERE YOU ARE.\n\n");
		new_location = old_location;
	}
	else
	{	// WHAT IF NOT ALL NEIGHBOURS AVAILABLE?
		float else_diffuse = diffusion_D *(1-0.5*state_of_chosen*bias);
		float main_diffuse = diffusion_D *(1+state_of_chosen*bias);
		if (else_diffuse <= 0 || main_diffuse <= 0)
		{
			printf("\n\nDiffusion rate must be positive.\n\n");
			free(old_neighbours);
			return;
		}
		else
		{
			diffusion_rate = rand_float_inrange(0, main_diffuse+2*else_diffuse);
			if (old_location % 2 ==0)
			{	// LOWER
				if (diffusion_rate > 0 && diffusion_rate < else_diffuse)
				{	// right
					new_location = old_neighbours[0];
				}
				else if (diffusion_rate > else_diffuse && diffusion_rate < 2*else_diffuse)
				{	// left
					new_location = old_neighbours[2];
				}
				else if (diffusion_rate > 2*else_diffuse && diffusion_rate < main_diffuse+2*else_diffuse)
				{	// top
					new_location = old_neighbours[1];
				}
			}
			else if (old_location % 2 == 1)
			{	// UPPER
				if (diffusion_rate > 0 && diffusion_rate < else_diffuse)
				{	// bottom
					new_location = old_neighbours[1];
				}
				else if (diffusion_rate > else_diffuse && diffusion_rate < 2*else_diffuse)
				{	// left
					new_location = old_neighbours[2];
				}
				else if (diffusion_rate > 2*else_diffuse && diffusion_rate < main_diffuse+2*else_diffuse)
				{	// right
					new_location = old_neighbours[0];
				}
			}
		}
	}
	free(old_neighbours);
	free(available_neighbours);
}

void free_site(struct site* s)
{
	free(s->onsite_agents);
	s->onsite_agents = NULL;
}

void print_site(struct site* s)
{
	printf("\n# population 0: %d", s->population_0);
	printf("\n# population 1: %d", s->population_1);
	printf("\n# population 2: %d", s->population_2);
	printf("\n# total population: %d", s->total_pop);
	for (int i = 0; i<s->total_pop; i++)
	{
		printf("\nAgent: %d, State: %d.", s->onsite_agents[i].location, s->onsite_agents[i].state);
	}
}

////////////////////////////////////////////// NEIGHBOURHOOD //////////////////////////////////////////////////
int *get_neighbours(int index, int number_row, int number_column)
{	// neighbours on triangular lattice
	int* neighbourhood;
	neighbourhood = (int*)malloc(3 * sizeof(int));

	// LOWER TRIANGLE
	if (index%2 == 0)
	{
		// TOP LEFT
		if (index == 0)
		{
			// right
			neighbourhood[0] = 1;
			// up
			neighbourhood[1] = number_row - 1;
			// left
			neighbourhood[2] = number_row*(number_column-1) + 1;
		}
		// LEFT BOUNDARY
		else if (index != 0 && index < number_row)
		{
			neighbourhood[0] = index + 1;
			neighbourhood[1] = index - 1;
			neighbourhood[2] = index + number_row*(number_column-1) + 1;
		}
		// TOP BPUNDARY
		else if (index != 0 && index%number_row == 0)
		{
			neighbourhood[0] = index + 1;
			neighbourhood[1] = index + number_row - 1;
			neighbourhood[2] = index - number_row + 1;
		}
		else
		{
			neighbourhood[0] = index + 1;
			neighbourhood[1] = index - 1;
			neighbourhood[2] = index - number_row + 1;
		}
	}

	// UPPER TRIANGLE
	else if (index%2 ==1)
	{
		// BOTTOM RIGHT
		if (index == number_row*number_column-1)
		{
			// right
			neighbourhood[0] = number_row - 2;
			// bottom
			neighbourhood[1] = index - number_row + 1;
			// left
			neighbourhood[2] = index - 1;
		}
		// RIGHT BOUNDARY
		else if (index != number_row*number_column-1 && index >number_row*(number_column-1))
		{
			neighbourhood[0] = index - number_row*(number_column-1) - 1;
			neighbourhood[1] = index + 1;
			neighbourhood[2] = index - 1;
		}
		// BOTTOM BOUNDARY
		else if (index != number_row*number_column-1 && (index+1) % number_row == 0)
		{
			neighbourhood[0] = index + number_row - 1;
			neighbourhood[1] = index - number_row + 1;
			neighbourhood[2] = index -1;
		}
		else
		{
			neighbourhood[0] = index + number_row - 1;
			neighbourhood[1] = index + 1;
			neighbourhood[2] = index - 1;
		}
	}

	return neighbourhood;
}

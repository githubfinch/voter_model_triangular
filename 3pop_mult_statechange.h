# include <stdio.h>
# include <stdlib.h>


struct agent
{
	int state;
	int location;
};

struct site
{
	int site_location;
	int population_0;
	int population_1;
	int population_2;
	struct agent *onsite_agents;
	int total_pop;
	// int overall_state;
};

struct agent create_agent(int index)
{
	struct agent new_agent;
	new_agent.state = rand()%3-1;
	new_agent.location = index;
	return new_agent;
}

void create_site(struct site* s, int site_loc, int tot_num_agent)
{
	s->site_location = site_loc;
	s->population_0 = 0;
	s->population_1 = 0;
	s->population_2 = 0;
	s->total_pop = tot_num_agent;
	s->onsite_agents = (struct agent*)malloc(tot_num_agent * sizeof(struct agent));
	for (int i = 0; i < tot_num_agent; i++)
	{
		s->onsite_agents[i] = create_agent(site_loc);
		if (s->onsite_agents[i].state == -1)
		{
			s->population_0++;
		}
		else if (s->onsite_agents[i].state == 0)
		{
			s->population_1++;
		}
		else if (s->onsite_agents[i].state == 1)
		{
			s->population_2++;
		}
	}
}

void add_agent_to(struct site *site, struct agent moving_agent)
{
	int rand_copy_index = -1;
	int change_state_to = -10;
	if (site->total_pop != 0)
	{
		printf("\nSITE TOTAL POP: %d.", site->total_pop);
		rand_copy_index = rand()%(site->total_pop);
		printf("\nSELECT INDEX TO COPY: %d",rand_copy_index);
		change_state_to = site->onsite_agents[rand_copy_index].state;
		printf("\nchange_state_to: %d.", change_state_to);
	}
	else if (site->total_pop == 0)
	{
		printf("\nNO ONE TO COPY, STAY SELF.");
	}
	struct agent *temp_agents = (struct agent*) realloc(site->onsite_agents, sizeof(struct agent)*(site->total_pop+1));
	if (temp_agents == 0)
	{
		printf("\nMEM ALLOC FAIL : add_agent_to");
		exit(1);
	}
	site->onsite_agents = temp_agents;
	// printf("\nREALOC MEM");
	// printf("\nmoving_agent.location: %d, site_location: %d",moving_agent.location,site->site_location);
	moving_agent.location = site->site_location;
	// printf("\nmoving_agent.location: %d, site_location: %d",moving_agent.location,site->site_location);
	if (rand_copy_index != -1)
	{
		moving_agent.state = change_state_to;
		printf("\nmoving_agent.state: %d.", moving_agent.state);
		site->onsite_agents[site->total_pop] = moving_agent;
	}
	else
	{
		printf("\nNo one to copy, moving_agent.state: %d.", moving_agent.state);
		site->onsite_agents[site->total_pop] = moving_agent;
	}
	switch(moving_agent.state)
	{
		case -1:
			site->population_0 ++;
			break;
		case 0:
			site->population_1 ++;
			break;
		case 1:
			site->population_2 ++;
			break;
	}
	site->total_pop ++;
}

void remove_agent_from(struct site *site, int agent_index)
{
	struct agent removed_agent = site->onsite_agents[agent_index];
	switch(removed_agent.state)
	{
		case -1:
			site->population_0 --;
			break;
		case 0:
			site->population_1 --;
			break;
		case 1:
			site->population_2 --;
			break;
	}
	site->total_pop --;
	if (site->total_pop == 0)
	{
		free(site->onsite_agents);
		site->onsite_agents = NULL;
		printf("\n\nSITE EMPTY\n");
	}
	else
	{
		for (int i = agent_index; i<site->total_pop; i++)
		{
			site->onsite_agents[i] = site->onsite_agents[i+1];
		}
		struct agent *temp_agents = (struct agent*)realloc(site->onsite_agents, sizeof(struct agent)*site->total_pop);
		if (temp_agents == NULL)
		{
			printf("\nMEM ALLOC FAIL or NO AGENTS LEFT: remove_agent_from\n");
			exit(1);
		}
		else
		{
			site->onsite_agents = temp_agents;
		}
	}
}

float rand_float_inrange(float start, float stop)
{
	float rand_num;
	float scale = rand()/(float)RAND_MAX;
	rand_num = start + scale*(stop-start);
	return rand_num;
}

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

void free_site(struct site* s)
{
	free(s->onsite_agents);
	s->onsite_agents = NULL;
}

void print_site(struct site* s)
{
	printf("\n# population -1: %d", s->population_0);
	printf("\n# population 0: %d", s->population_1);
	printf("\n# population 1: %d", s->population_2);
	printf("\n# total population: %d", s->total_pop);
	for (int i = 0; i<s->total_pop; i++)
	{
		printf("\nAgent on site %d, State: %d.", s->onsite_agents[i].location, s->onsite_agents[i].state);
	}
}

void hop_from(int old_location, struct site* lattice, float diffusion_D, float bias, int num_row, int num_col)
{
	int chosen_agent = rand() % lattice[old_location].total_pop;
	struct agent moving_agent = lattice[old_location].onsite_agents[chosen_agent];
	int state_of_chosen = moving_agent.state;
	int new_location;
	float hop_rate;
	int *old_neighbours = get_neighbours(old_location,num_row,num_col);

	float else_diffuse = diffusion_D *(1-0.5*state_of_chosen*bias);
	float main_diffuse = diffusion_D *(1+state_of_chosen*bias);

	if (else_diffuse <= 0 || main_diffuse <= 0)
	{
		printf("\n\nDiffusion rate must be positive.\n\n");
		free(old_neighbours);
		exit(1);
	}
	else
	{
		hop_rate = rand_float_inrange(0, main_diffuse+2*else_diffuse);
		printf("\nHOP RATE: %f", hop_rate);
		if (old_location % 2 ==0)
		{	// LOWER
			if (hop_rate > 0 && hop_rate < else_diffuse)
			{	// right
				new_location = old_neighbours[0];
			}
			else if (hop_rate > else_diffuse && hop_rate < 2*else_diffuse)
			{	// left
				new_location = old_neighbours[2];
			}
			else if (hop_rate > 2*else_diffuse && hop_rate < main_diffuse+2*else_diffuse)
			{	// top
				new_location = old_neighbours[1];
			}
		}
		else if (old_location % 2 == 1)
		{	// UPPER
			if (hop_rate > 0 && hop_rate < else_diffuse)
			{	// bottom
				new_location = old_neighbours[1];
			}
			else if (hop_rate > else_diffuse && hop_rate < 2*else_diffuse)
			{	// left
				new_location = old_neighbours[2];
			}
			else if (hop_rate > 2*else_diffuse && hop_rate < main_diffuse+2*else_diffuse)
			{	// right
				new_location = old_neighbours[0];
			}
		}
		printf("\nNEW LOCATION of AGENT %d: %d. WAS AT: %d. \n", chosen_agent, new_location, old_location);
	}
	// printf("\nelse_diffuse: %f, main diffuse: %f", else_diffuse, main_diffuse);
	printf("\n");
	print_site(&lattice[new_location]);
	printf("\n");
	add_agent_to(&lattice[new_location], moving_agent);
	printf("\n");
	print_site(&lattice[new_location]);
	printf("\nAGENT %d with STATE %d moved to SITE %d",chosen_agent, moving_agent.state, lattice[new_location].site_location);
	remove_agent_from(&lattice[old_location], chosen_agent);
	printf("\nAGENT %d REMOVED from SITE %d",chosen_agent, old_location);

	free(old_neighbours);
}
# include <stdio.h>
# include <stdlib.h>

struct Node{
	int data;
	struct Node* next;
	struct Node* next_occur;
};
//---------------------------------------------------------------------------------
struct Node* createNode(int data){
	struct Node* new_Node = (struct Node*)malloc(sizeof(struct Node));
	if (new_Node == NULL){
		fprintf(stderr, "MEM ALLOC FAIL!!!\n");
		exit(EXIT_FAILURE);
	}
	new_Node->data = data;
	new_Node->next = NULL;
	new_Node->next_occur = NULL;
	return new_Node;
}
//---------------------------------------------------------------------------------
struct Node* createList(int* arr, int size){ // ARRAY -> LIST
	struct Node* head = NULL;
	struct Node* tail = NULL;
	for (int i = 0; i < size; i++)
	{
		struct Node * temp = createNode(arr[i]);
		if (head == NULL)
		{
			head = temp;
			tail = temp;
		}
		else
		{
			tail->next = temp;
			tail = temp;
		}
	}
	return head;
}
//---------------------------------------------------------------------------------
int get_list_data(struct Node* head, int index)
{
	struct Node* temp = head;
	for (int c = 0; c<index; c++)
	{
		temp = temp->next;
	}
	return temp->data;
}
//---------------------------------------------------------------------------------
int list_length(struct Node* head)
{
	struct Node* current = head;
	int length = 0;
	while(current != NULL)
	{
		length++;
		current = current->next;
	}
	return length;
}
//---------------------------------------------------------------------------------
void link_occurs(struct Node* head)
{
	struct Node* current = head;

	while (current != NULL){
		struct Node* temp = current->next;
		while (temp != NULL){
			if (temp->data == current->data){
				current->next_occur = temp;
				break;
			}
			temp = temp->next;
		}
		current = current->next;
	}
}
//---------------------------------------------------------------------------------
void print_list(struct Node* head)
{
	int index = 0;
	struct Node* current = head;
	while (current != NULL)
	{
		printf("INDEX: %d, DATA: %d", index, current->data);
		if (current->next_occur != NULL)
		{
			struct Node* temp = head;
			int next_index = 0;
			while (temp != NULL)
			{
				if (temp == current->next_occur)
				{
					printf(", NEXT AT ID: %d", next_index);
					break;
				}
				temp = temp->next;
				next_index++;
			}
		}
		else
		{
			printf(", NO MORE OCCURRENCE");
		}
		printf("\n");
		current = current->next;
		index++;
	}
}
//---------------------------------------------------------------------------------
int* isolate_list(struct Node* head, int chain_head_value, int* count)
{	// returns indices of instances in the list
	int index = 0;
	int* indices = NULL;
	*count = 0;
	struct Node* current = head;

	while (current != NULL)
	{
		// printf("CHECK4\n");

		if (current->data == chain_head_value)
		{
			indices = realloc(indices, (*count + 1) * sizeof(int));

			if (indices == NULL)
			{
				fprintf(stderr, "MEM ALLOC FAIL!!!\n");
				exit(EXIT_FAILURE);
			}
			indices[*count] = index;
			if (index%2 == 0) // index is head of pair
			{
				indices[*count+1] = index + 1;
			}
			else if (index%2 == 1) // index is second of pair
			{
				indices[*count+1] = index - 1;
			}
			(*count)+=2;
		}
		current = current->next;
		index++;
	}
	return indices;
}
//---------------------------------------------------------------------------------
void remove_node(struct Node** head_ref, int index)
{
	if (*head_ref == NULL){
		return;
	}

	struct Node* temp = *head_ref;
	if (index == 0){
		*head_ref = temp->next;
		free(temp);
		return;
	}
	for (int i =0; i<index-1 && temp != NULL; i++){
		temp = temp->next;
		if (temp == NULL || temp->next == NULL){
			return;
		}
	}
	struct Node* next = temp->next->next;
	free(temp->next);
	temp->next = next;
	printf("\nnodes removed.");
}

void append_node(struct Node** head_ref, int data)
{
	struct Node* new_node = createNode(data);
	if (*head_ref == NULL)
	{
		*head_ref = new_node;
		return;
	}
	struct Node* last = *head_ref;
	while (last->next != NULL)
	{
		last = last->next;
	}
	last->next = new_node;
	// int length = list_length(head);
	// for (int i = 0; i<length; i++)
	// {
	// 	current = current->next;
	// }
	// current->next = new_node;
}
//---------------------------------------------------------------------------------
int rps_game(int* list_of_states, int size_of_list, int node1_id, int node2_id)
{	// 0->1->2->0
	int conformed;
	printf("nodes: %d,%d", node1_id,node2_id);
	printf("\nLength of list: %d\n",size_of_list);
	for (int i = 0; i<size_of_list;i++)
	{
		printf("%d,",list_of_states[i]);
	}

	if (node1_id>size_of_list-1)
	{
		perror("INDEX 1 OUT OF BOUNDS\n");
		return 1;
	}
	else if (node2_id>size_of_list-1)
	{
		perror("INDEX 2 OUT OF BOUNDS\n");
		return 1;
	}

	if (list_of_states[node1_id] == 0)
	{
		printf("list_of_states[node1_id]: %d\n",list_of_states[node1_id]);
		if (list_of_states[node2_id] == 1)
		{	// 0->1
			list_of_states[node2_id] = list_of_states[node1_id];
			conformed = node2_id;
			printf("conformed:%d\n", conformed);
		}
		else if (list_of_states[node2_id] == 2)
		{	// 0<-2
			list_of_states[node1_id] = list_of_states[node2_id];
			conformed = node1_id;
			printf("conformed:%d\n", conformed);
		}
	}
	else if (list_of_states[node1_id] == 1)
	{
		printf("list_of_states[node1_id]: %d\n",list_of_states[node1_id]);
		if (list_of_states[node2_id] == 2)
		{	// 1->2
			list_of_states[node2_id] = list_of_states[node1_id];
			conformed = node2_id;
			printf("conformed:%d\n", conformed);
		}
		else if (list_of_states[node2_id] == 0)
		{	// 1<-0
			list_of_states[node1_id] = list_of_states[node2_id];
			conformed = node1_id;
			printf("conformed:%d\n", conformed);
		}
	}
	else if (list_of_states[node1_id] == 2)
	{
		printf("list_of_states[node1_id]: %d\n",list_of_states[node1_id]);
		if (list_of_states[node2_id] ==0)
		{	// 2->0
			list_of_states[node2_id] = list_of_states[node1_id];
			conformed = node2_id;
			printf("conformed:%d\n", conformed);
		}
		else if (list_of_states[node2_id] == 1)
		{	// 2<-1
			list_of_states[node1_id] = list_of_states[node2_id];
			conformed = node1_id;
			printf("conformed:%d\n", conformed);
		}
	}
	printf("\ngame played, conformed index: %d", conformed);
	return conformed;
}
//---------------------------------------------------------------------------------
void free_list(struct Node* head){
	struct Node* temp;
	while (head != NULL){
		temp = head;
		head = head->next;
		free(temp);
	}
}
////////////////////////////////////////////// NEIGHBOURHOOD //////////////////////////////////////////////////
int *get_neighbours(int index, int number_row, int number_column)
{
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

int picky_neighbour(int* list_of_states, int index, int number_row, int number_column)
{
	int food_id;

	// state 0 - __________________________________________________________________
	if (list_of_states[index] == 0)
	{	// LOWER
		if (index%2 == 0)
		{	// top row
			if (index%number_row == 0)
			{
				food_id = index + number_row-1;
			}
			// else
			else
			{
				food_id = index - 1;
			}
		}
		// UPPER
		else if (index%2 == 1)
		{	// bottom row
			if((index+1)%number_row == 0)
			{
				food_id = index - number_row + 1;
			}
			// else
			else
			{
				food_id = index + 1;
			}
		}
	}
	// state 1 forward slash __________________________________________________________________
	else if (list_of_states[index] == 1)
	{
		if (index%2 == 0)
		{	// left column
			if (index < number_row)
			{
				food_id = index + (number_column-1)*number_row + 1;
			}
			// else
			else
			{
				food_id = index - number_row + 1;
			}
		}
		// UPPER
		else if (index%2 == 1)
		{	// right column
			if(index > (number_column-1)*number_row)
			{
				food_id = index - number_row*(number_column-1) - 1;
			}
			// else
			else
			{
				food_id = index + number_row - 1;
			}
		}
	}
	// state 2 / __________________________________________________________________
	else if (list_of_states[index] == 2)
	{
		if (index%2 == 0)
		{
			food_id = index + 1;
		}
		else if (index%2 == 1)
		{
			food_id = index - 1;
		}
	}
	return food_id;
}
import matplotlib.pyplot as plt
import numpy as np

def upper_triangle(centre_x, centre_y, side_length, state):
    vertex_x = [centre_x, centre_x + side_length/2, centre_x - side_length/2, centre_x]
    vertex_y = [centre_y + side_length/np.sqrt(3), centre_y - side_length/(2*np.sqrt(3)), 
                centre_y - side_length/(2*np.sqrt(3)), centre_y + side_length/np.sqrt(3)]
    # TOP, RIGHT, LEFT, TOP to close shape

    if state == 0:
        plt.fill(vertex_x, vertex_y, "darkorange")
    elif state == 1:
        plt.fill(vertex_x, vertex_y, "yellowgreen")
    elif state == 2:
        plt.fill(vertex_x, vertex_y, "cornflowerblue")

def lower_triangle(centre_x, centre_y, side_length, state):
    vertex_x = [centre_x, centre_x + side_length/2, centre_x - side_length/2, centre_x]
    vertex_y = [centre_y - side_length/np.sqrt(3), centre_y + side_length/(2*np.sqrt(3)), 
                centre_y + side_length/(2*np.sqrt(3)), centre_y - side_length/np.sqrt(3)]
    # BOTTOM, RIGHT, LEFT, BOTTOM to close shape

    if state == 0:
        plt.fill(vertex_x, vertex_y, "darkorange")
    elif state == 1:
        plt.fill(vertex_x, vertex_y, "yellowgreen")
    elif state == 2:
        plt.fill(vertex_x, vertex_y, "cornflowerblue")
        


STATES = []
# with (open('states_BASIC.txt','r')) as file:
with (open('states_RPS.txt','r')) as file:
# with (open('states_PICKY.txt','r')) as file:
    for line in file:
        STATE = [int(char) for char in line.strip()]
        STATES.append(STATE)

# number_of_rows = 50
# number_of_columns = 25

number_of_rows = 80
number_of_columns = 40


start_x = 0
start_y = 0
side_length = 1
a = side_length/(2*np.sqrt(3))
b = side_length/2
c = 2*a

it_cell = 0
it_row = 0
it_col = 0

sample_head = 60000
sample_tail = 87000
sample_rate = 20
sampling = np.linspace(sample_head,sample_tail,num=sample_rate,endpoint=True)

plt.figure()
for i in range(sample_rate):
    state = STATES[int(sampling[i])]
    for it_col in range(number_of_columns):
        for it_row in range(number_of_rows):
            if it_row % 2 == 0:
                # lower_triangle(start_x + (it_row/2+2*it_col+1) * b, 
                               # start_y - 3*(it_row/2) * a, side_length, state[it_cell])
                lower_triangle(start_x + (it_row/2+2*it_col+1) * b, 
                               start_y - 3*(it_row/2) * a, side_length, state[it_cell])
            if it_row % 2 == 1:
                # upper_triangle(start_x +b+ ((it_row+1)/2+2*it_col) * b, 
                               # start_y - (3*(it_row+1)/2-2) * a, side_length, state[it_cell])
                upper_triangle(start_x +b+ ((it_row+1)/2+2*it_col) * b, 
                                start_y - (3*(it_row+1)/2-2) * a, side_length, state[it_cell])
            it_cell = it_cell+1
    it_cell = 0
    plt.gca().set_aspect('equal', adjustable = 'box')
    plt.axis('off')
    plt.text(0.15, 0.15, f" {int(sampling[i])}", ha='center', va='top', fontsize=11, color='black', transform=plt.gca().transAxes)
    plt.savefig('state_{}_rps80x40srand10.png'.format(int(sampling[i])), dpi=200, bbox_inches = 'tight')
    plt.show()
# plt.savefig('final_state_PICKY10x5_srand10.png', dpi=200, bbox_inches = 'tight')
# plt.savefig('final_state_PICKY16x8_srand10.png', dpi=200, bbox_inches = 'tight')
# plt.savefig('final_state_PICKY50x25_srand10.png', dpi=200, bbox_inches = 'tight')
# upper_triangle(3, 2, 1, 2)
# lower_triangle(3, 2, 1, 1)

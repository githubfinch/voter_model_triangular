#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Nov 26 21:11:10 2024

@author: apple
"""
# from tqdm import tqdm
import matplotlib.pyplot as plt
from matplotlib.pyplot import figure
import numpy as np
states = []
with (open('3_pop_RPS.txt','r')) as file:
# with (open('3_pop_BASIC.txt','r')) as file:
# with (open('3_pop_PICKY.txt','r')) as file:
    for line in file:
        rps_state = line.rstrip().split(',')
        states.append([float(val) for val in rps_state])
        print(rps_state)

states_array = np.array(states)
time_step = np.linspace(0,len(states),len(states))

fig, ax = plt.subplots(figsize = (16,4))

scatter_start = 60000
ax.scatter(time_step[scatter_start:],states_array[scatter_start:,0],s=0.1,c='#c65102')
ax.scatter(time_step[scatter_start:],states_array[scatter_start:,1],s=0.1,c='#9aae07')      # CHANGE MARKER SIZE
ax.scatter(time_step[scatter_start:],states_array[scatter_start:,2],s=0.1,c='#047495')

all_values = np.array(states).flatten()

ax.set_ylim(np.min(all_values) - 1, np.max(all_values) + 1)
# Avoid multiple legends by setting them only once
ax.legend(loc='best')

# Labeling the axes and title
ax.legend(['state 0','state1','state2'], markerscale = 5)
ax.set_xlabel('Time Step')
ax.set_ylabel('Population size')

# ax.set_title('10x5 picky model')
# ax.set_title('16x8 picky model')
# ax.set_title('20x10 picky model')

# fig.savefig('picky_10-5_srand10.png',dpi=200)
# fig.savefig('picky_16-8_srand10.png',dpi=200)
# fig.savefig('picky_20-10_srand10.png',dpi=200)

# ax.set_title('10x5 RPS model')
# ax.set_title('16x8 RPS model')
# ax.set_title('20x10 RPS model')
# ax.set_title('50x25 RPS model')
ax.set_title('80x40 RPS model')


# fig.savefig('rps_10-5.png',dpi=200)
# fig.savefig('rps_16-8.png',dpi=200)
# fig.savefig('rps_20-10_srand10.png',dpi=200)
# fig.savefig('rps_50-25srand10.png',dpi=500)
fig.savefig('rps_80-40srand10.png',dpi=200)


# ax.set_title('10x5 basic model')
# ax.set_title('16x8 basic model')
# ax.set_title('20x10 basic model')
# ax.set_title('50x25 basic model')

# fig.savefig('basic_10-5.png',dpi=200)
# fig.savefig('basic_16-8.png',dpi=200)
# fig.savefig('basic_20-10srand10.png',dpi=200)
# fig.savefig('basic_50-25srand10.png',dpi=200)


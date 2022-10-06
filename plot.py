from matplotlib import pyplot as plt
import matplotlib as mpl
import numpy as np
import pandas as pd

plt.style.use('ggplot')

mpl.rcParams['legend.fontsize'] = 14

font = {'family': 'Times New Roman',  # 'serif',
        'color': 'darkblue',
        'weight': 'normal',
        'size': 14}

# read in data
try:
    f_name = './algorithm.dat'
    with open(f_name) as _:
        ...
    print('found ' + f_name)
    data = pd.read_csv(f_name, header=None, names=['1', '2', '3', '4', '5'])

except FileNotFoundError as e:
    f_name = './cmake-build-debug/algorithm.dat'
    print(f'found {f_name}')
    data = pd.read_csv(f_name, header=None, names=['1', '2', '3', '4', '5'])

print('[Python] Read in data...')


d1 = data['1']
d2 = data['2']
d3 = data['3']
d4 = data['4']
d5 = data['5']
time = np.arange(10000)

fig, ax = plt.subplots(ncols=1, nrows=3, sharex=True, figsize=(16 * 0.8, 9 * 0.8), dpi=80,
                         facecolor='w', edgecolor='k')
# fig, (ax1, ax2, ax3) = plt.subplots(nrows=3, ncols=1, figsize=(6, 3), dpi=200)
fig.subplots_adjust(right=0.95, bottom=0.1, top=0.95, hspace=0.2, wspace=0.02)

ax[0].plot(time, d1, linewidth=1)
ax[0].plot(time, d2, linewidth=1)
ax[0].set_xticks([])
ax[1].plot(time, d3, linewidth=1)
ax[1].plot(time, d4, linewidth=1)
ax[1].set_xticks([])
ax[2].plot(time, d5, linewidth=0.7)

ax[0].set_xlim(-100, 10100)
ax[1].set_xlim(-100, 10100)
ax[2].set_xlim(-100, 10100)
# data[4].plot()
# plt.constrained_layout = True,
# subplots=True, layout=(5, 1),

# ax_list = plt.get_axis((1, 3))
# plt.plot_it(ax_list[0], r'$i_s$ [V]')
# plt.plot_it(ax_list[1], r'$\psi_\mu$ [A]')
# plt.plot_it(ax_list[2], r'speed [rpm]')
# Automatic END
plt.show()

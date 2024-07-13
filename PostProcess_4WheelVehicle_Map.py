import numpy as np
from matplotlib import pyplot as plt

### Filenames for data
datafiles = ["x0[mps]", "x1[mps]", "x2[dps]", "x3[m]", "x4[m]", "x5[deg]"]

def add_arrow(line, position=None, direction='right', size=15, color=None):
    """
    add an arrow to a line.

    line:       Line2D object
    position:   x-position of the arrow. If None, mean of xdata is taken
    direction:  'left' or 'right'
    size:       size of the arrow in fontsize points
    color:      if None, line color is taken.
    """
    if color is None:
        color = line.get_color()

    xdata = line.get_xdata()
    ydata = line.get_ydata()

    if position is None:
        position = xdata.mean()
    # find closest index
    start_ind = np.argmin(np.absolute(xdata - position))
    if direction == 'right':
        end_ind = start_ind + 1
    else:
        end_ind = start_ind - 1

    line.axes.annotate('',
        xytext=(xdata[start_ind], ydata[start_ind]),
        xy=(xdata[end_ind], ydata[end_ind]),
        arrowprops=dict(arrowstyle="->", color=color),
        size=size
    )

### Plot travel on XY map
plt.style.use("dark_background")
plt.figure(1)

# Open data files in binary mode and save to np arrays
with open("./log/" + datafiles[3]+".log", 'rb') as f:
	# Read the time data into a NumPy array
	Xdata = np.fromfile(f, dtype=np.float64)  # Change dtype according to your data
	f.close() # close file
with open("./log/" + datafiles[4]+".log", 'rb') as f:
	# Read the time data into a NumPy array
	Ydata = np.fromfile(f, dtype=np.float64)  # Change dtype according to your data
	f.close() # close file

# Plot data
plt.rc('grid', linestyle="dotted", color='grey') 
plt.subplot(1, 1, 1, facecolor='#1C1C1C')
plt.title("XY Map", fontweight='bold')
plt.ylabel("X [m]", fontweight='bold')
plt.xlabel("Y [m]", fontweight='bold')
ax = plt.gca()
ax.set_aspect('equal', adjustable='box')
traj = plt.plot(Ydata, Xdata, color='red')[0] # Y is on x-axis, X is on y-axis
add_arrow(traj)
plt.grid()

# Plot circular path
circle = plt.Circle((0, 0), 10.0, fill = False)
ax.add_artist( circle )

# Show the plot (maximized)
#mng = plt.get_current_fig_manager()
#mng.window.showMaximized()
#plt.show()

# Save the plot
plt.savefig('./figs/map.png', dpi = 1000)
plt.close(1)
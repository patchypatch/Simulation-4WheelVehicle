import numpy as np
from matplotlib import pyplot as plt

### Filenames for data
datafiles = ["error0[m]", "error1[deg]"]

### Open time data file in binary mode and save to np array
with open("./log/time[s].log", 'rb') as f:
	# Read the time data into a NumPy array
	time = np.fromfile(f, dtype=np.float64)  # Change dtype according to your data
	f.close() # close file

### Plot states vs. time 
plt.style.use("dark_background")
plt.figure(0)
for i in datafiles:
	# Open data file in binary mode and save to np array
	with open("./log/" + i + ".log", 'rb') as f:
		# Read the data into a NumPy array
		data = np.fromfile(f, dtype=np.float64)  # Change dtype according to your data
	f.close() # close file

	# Plot data
	plt.rc('grid', linestyle="dotted", color='grey') 
	plt.subplot(2, 1, datafiles.index(i)*1+1, facecolor='#1C1C1C')
	plt.ylabel(i, fontweight='bold')
	plt.xlabel("Time[s]", fontweight='bold')
	plt.plot(time, data, color='#c95c0e')
	plt.grid()

# Show the plot (maximized)
#mng = plt.get_current_fig_manager()
#mng.window.showMaximized()
#plt.show()

# Save the plot
plt.savefig('./figs/errors.png', dpi = 1000)
plt.close(0)
import plotly.graph_objects as go

import numpy as np

# To slow down simulation
AniSpeedMultiplier = 1; # ~ seconds animation / seconds simulated 
G = int(10 / AniSpeedMultiplier)

### Filenames for data
datafiles = ["x0[mps]", "x1[mps]", "x2[dps]", "x3[m]", "x4[m]", "x5[deg]"]

### Open time data file in binary mode and save to np array
with open("./log/time[s].log", 'rb') as f:
	# Read the time data into a NumPy array
	t = np.fromfile(f, dtype=np.float64)  # Change dtype according to your data
	f.close() # close file

### Extract data from logs
x = [] 
for i in datafiles:
	# Open data file in binary mode and save to np array
	with open("./log/" + i + ".log", 'rb') as f:
		# Read the data into a NumPy array
		x.append( np.fromfile(f, dtype=np.float64) )  # Change dtype according to your data
	f.close() # close file

# Generate curve data
xm = np.min(x[4]) - 1.5
xM = np.max(x[4]) + 1.5
ym = np.min(x[3]) - 1.5
yM = np.max(x[3]) + 1.5

iend = x[3][0::G] + x[0][0::G]*np.cos(np.deg2rad(x[5][0::G]))  # end coordinates for the body vx vector
jend = x[4][0::G] + x[0][0::G]*np.sin(np.deg2rad(x[5][0::G]))

inoe = x[3][0::G] - x[1][0::G]*np.sin(np.deg2rad(x[5][0::G]))  # end coordinates for the body vy vector
jnoe = x[4][0::G] + x[1][0::G]*np.cos(np.deg2rad(x[5][0::G]))

# Create figure
fig = go.Figure(
    data=[go.Scatter(x=x[4][0::G], y=x[3][0::G],
                     name="frame",
                     mode="lines",
                     line=dict(width=1, color="red")),
          go.Scatter(x=x[4][0::G], y=x[3][0::G],
                     name="curve",
                     mode="lines",
                     line=dict(width=1, color="red"))
          ],
    layout=go.Layout(width=800, height=800,
                     xaxis=dict(range=[xm, xM], autorange=False, zeroline=False),
                     yaxis=dict(range=[ym, yM], autorange=False, zeroline=False),
                     plot_bgcolor='white',
                     title="Vehicle Body-Frame Trajectory Animation",
                     hovermode="closest",
                     updatemenus=[dict(type="buttons",
                                       buttons=[dict(label="Play",
                                                     method="animate",
                                                     args=[None, {"frame": {"duration": 10, "redraw": False},}])])]),
    frames=[go.Frame(
        data=[go.Scatter(
            x=[x[4][0::G][k], jend[k], None, x[4][0::G][k], jnoe[k]], # x-axis is Y
            y=[x[3][0::G][k], iend[k], None, x[3][0::G][k], inoe[k]], # y-axis is X
            mode="lines",
            line=dict(color="#c2c930", width=2))
        ]) for k in range((t[0::G]).size)]
)

# make axes have equal scales
fig.update_yaxes(
    scaleanchor="x",
	scaleratio=1,
    )

# make grid grey
fig.update_yaxes(
	gridcolor='grey',
	gridwidth=1
)
fig.update_xaxes(
	gridcolor='grey',
    gridwidth=1
)

# show figure
fig.show()
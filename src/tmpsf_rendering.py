import json
import plotly.graph_objects as go

# reading thermistor locations
x = []
y = []
z = []

with open('../thermistor_pos.json') as json_file:
    data = json.load(json_file)
    for thermistor in data:
        x.append(data[thermistor][0])
        y.append(data[thermistor][1])
        z.append(data[thermistor][2])
fig = go.Figure(data=[go.Scatter3d(x=x, y=y, z=z, mode='markers')])
fig.show()

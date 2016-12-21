#! /bin/python3
def toFloats(line):
	stringList = line.split(',')
	intList = []
	for i in range(0,len(stringList)):
		intList.append(float(stringList[i]))
	return intList

import plotly
from plotly.graph_objs import Scatter, Layout

fileData = []
with open("dataToPlot.txt") as d:
	fileData = d.read().splitlines()

xcoord = toFloats(fileData[1])
ycoord = toFloats(fileData[2])

plotly.offline.plot({
	"data": [Scatter(x=xcoord, y=ycoord)],
	"layout": Layout(title=fileData[0])
	})

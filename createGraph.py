#! /bin/python3

import sys

def toFloats(line):
	stringList = line.split(',')
	intList = []
	for i in range(0,len(stringList)):
		intList.append(float(stringList[i]))
	return intList

import plotly
from plotly.graph_objs import Scatter, Layout, Bar

def makeScatter(title,xcoord,ycoord):
	plotly.offline.plot({
		"data": [Scatter(x=xcoord, y=ycoord)],
		"layout": Layout(title=title)
		})

def makeBar(title,xcoord,ycoord):
	plotly.offline.plot({
		"data": [Bar(x=xcoord, y=ycoord)],
		"layout": Layout(title=title)
		})

def loadScatterCoords(fileData,xcoord,ycoord):
	for i in range(1,len(fileData)):
		stringList = fileData[i].split(',')
		print(stringList)
		xcoord.append(float(stringList[0]))
		ycoord.append(float(stringList[1]))

def loadBarCoords(fileData,xcoord,ycoord):
	for i in range(1,len(fileData)):
		stringList = fileData[i].split(',')
		print(stringList)
		xcoord.append(stringList[0])
		ycoord.append(int(stringList[1]))

def __main__():
	print(len(sys.argv))
	if (len(sys.argv)>1):
		gType = sys.argv[1]
		if (len(sys.argv)>2):
			fToOpen = sys.argv[2]
		else:
			fToOpen = "dataToPlot.txt"
	else:
		fToOpen = "dataToPlot.txt"
		gType = "Scatter"

	fileData = []
	with open(fToOpen) as d:
		fileData = d.read().splitlines()

	xcoord = []
	ycoord = []
	if (gType == "Scatter"):
		loadScatterCoords(fileData,xcoord,ycoord)
		makeScatter(fileData[0],xcoord,ycoord)
	elif (gType == "Bar"):
		loadBarCoords(fileData,xcoord,ycoord)
		makeBar(fileData[0],xcoord,ycoord)


__main__()
# import plotly.plotly as py
# import plotly.graph_objs as go

# data = [go.Bar(
#             x=['giraffes', 'orangutans', 'monkeys'],
#             y=[20, 14, 23]
#     )]

# py.iplot(data, filename='basic-bar')
#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Stephen Stengel  <stephen.stengel@cwu.edu>  40819903
#
# Plotting script.

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy
import os


def main(args):
	myStuff = []
	for i in range(0, 20):
		myStuff.append( [i, i*2, i*3] )
	
	filename = "testfile.txt"
	printListListToFile(myStuff, filename)
	
	data = readListListFromFile(filename)
	# ~ data = readListListFromFile("real-test-tupples")
	
	print(data)
	#split into x y z arrays.
	X, Y, Z = convertToThreeArrays(data)

	picOutName = "graph-test"
	printGraph(X, Y, Z, "A TITLE lol", "xLable", "yLable", "ZLable", picOutName)
	os.system("sync")
	os.system("optipng ../pics/*.png")
	
	return 0


def convertToThreeArrays(data):
	X, Y, Z = map(list, zip(*data))
	X = numpy.array(X, dtype=numpy.float32)
	Y = numpy.array(Y, dtype=numpy.float32)
	Z = numpy.array(Z, dtype=numpy.float32)
	
	return X, Y, Z


#Prints a list of tupples to file.
def printListListToFile(myTupples, filename):
	with open( "./data/" + filename, "w") as myFile:
		for thingy in myTupples:
			for i in range(0, len(thingy)):
				myFile.write(f"{thingy[i]}")
				if (i + 1) != len(thingy):
					myFile.write(" ")
				else:
					myFile.write("\n")

def readListListFromFile(filename):
	outList = []
	with open(filename, "r") as myFile:
		for line in myFile:
			thisList = line.rstrip().split(" ")
			outList.append(thisList)

	return outList


def printGraph(xArray, yArray, zArray, title, xLab, yLab, zLab, filename):
	myFigure = plt.figure()
	myFrame = myFigure.add_subplot(111, projection='3d')
	# ~ myFrame.scatter(xArray, yArray, zArray, marker = "o")
	myFrame.plot_trisurf(xArray, yArray, zArray)
	
	
	# ~ myFrame.scatter(xArray, yArray, zArray, marker = "o", label = "TEST LABEL LOL")
	# ~ myFrame.scatter(xArrayAho, yArrayAho, zArrayAho, marker = "^", label = "Aho Corasick")
	myFrame.set_title(title)
	myFrame.set_xlabel(xLab)
	myFrame.set_ylabel(yLab)
	myFrame.set_zlabel(zLab)
	
	# ~ myFrame.axes.set_xlim3d(left=0, right=200) 
	
	# ~ myFrame.legend()
	# ~ myFrame.legend(loc = 6, ncol = 1)
	
	plt.savefig("./pics/" + filename + ".png")
	# ~ plt.show()#
	plt.clf()


if __name__ == '__main__':
	import sys
	sys.exit(main(sys.argv))

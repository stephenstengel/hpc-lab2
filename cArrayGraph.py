#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Stephen Stengel  <stephen.stengel@cwu.edu>  40819903

#C array grapher

import sys
import os

import plotting


def main(args):
	if len(args) != 4:
		print("bad input");
		sys.exit(-1)
	else:
		filename = sys.argv[1]
		picFile = sys.argv[2]
		testType = sys.argv[3]
	
	#m n t time name
	data = plotting.readListListFromFile(filename)
	
	# ~ X, Y, Z = plotting.convertToThreeArrays(data)
	M, N, T, TIME, NAME = plotting.convertToFiveArrays(data)
	
	print("Still working!")
	
	
	plotting.printGraph(M, T, TIME,
			"Time for " + str(testType) + " Method.",
			"Square Size (M)", "Threads (T)", "Time in seconds", picFile)
	os.system("sync")
	# ~ os.system(f"optipng ../pics/{picOutName}.png")
	
	return 0

if __name__ == '__main__':
	import sys
	sys.exit(main(sys.argv))

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Stephen Stengel  <stephen.stengel@cwu.edu>  40819903

#C array grapher

import sys
import os

import plotting


def main(args):
	if len(args) != 2:
		print("bad input");
		sys.exit(-1)
	else:
		filename = sys.argv[1]
	
	data = plotting.readListListFromFile(filename)
	
	X, Y, Z = plotting.convertToThreeArrays(data)
	
	# ~ picOutName = "lab1-CARRAY" + filename[8:]
	picOutName = filename[8:]
	plotting.printGraph(X, Y, Z,
			"C Array Time for Each Test. (Min of three trials)",
			"Square Size (M)", "Multiplications (N)", "Time in seconds", picOutName)
	os.system("sync")
	os.system(f"optipng ../pics/{picOutName}.png")
	
	return 0

if __name__ == '__main__':
	import sys
	sys.exit(main(sys.argv))

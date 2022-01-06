#Stephen Stengel <stephen.stengel@cwu.edu> 40819903
#makefile for lab2

#Make executable. No -O2 optimization
lab2: lab2-stephen-stengel.o
	gcc -Wall -fopenmp -o lab2 lab2-stephen-stengel.o -lm


#Make the object file
lab2-stephen-stengel.o: lab2-stephen-stengel.c lab2-stephen-stengel.h mylab1functions.h
	gcc -Wall -fopenmp -c lab2-stephen-stengel.c -lm


#Miscellaneous
.PHONY: clean
clean:
	rm -f lab2 *.o lab2-stephen-stengel.zip

.PHONY: backup
backup:
	ssh-backup-pi | lolcat

.PHONY: zip
zip:
	7z a lab2-stephen-stengel.zip *.c *.h *.py Makefile helpfile \
			../paper/stephen-stengel-lab2.pdf ../video/*.mkv ../pics \
			../data
	
	##README

.PHONY: script
script:
	make clean && make && make zip && make backup

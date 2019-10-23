# Homework 4

Documentation

In General, there a single Makefile located in root directory that take cares of all 
of the programs

part2:
	the make commands follow the require format.
	make progfib
	make progcnt
	make progqsort

	hw4_progfib N, launch a single thread to calculate the Nth fibonacci number

	hw4_progcnt, launches three thread that sum up the occurence of 0, 1, 2 in files: 
	file1.dat, file2.dat, file3.dat which are located in root directory.

	hw4_progqsort filename.dat, lauches parallel quicksort thread that quicksort of 
	the given file.

part3:
	make part3, creates an executable call artist_manager
	
	artist_manager simulates the process of hiring and firing artist throw threads.
	(Each artist is a thread)

	artist_ds.c contains the need data struct and data structure function require to
	perform each command.

part4:
	make, creates an executable call museum_overseer

	museum_overseer, simulates a garbage collector through instances of museums and thread
	museum_ds is the struct that contains infomation of a museum.

	There is 5 museum in total in the form of an array.

	visitor in(int N). This function imitates that a visitor entered museum number N. 

	visitor out(int N). This function imitates that a visitor left museum number N.
	(note negative visitor will be set back to 0)

	museum_info(). This function prints each museum, <museum #>:<visitor>:<status>

	museum_close(int N). This function closes museum N

	museum_clean(), This function kicks out all visitors from all museums if
	there are any, closes all museums, and frees all the memory.

	we used the writer-reader approach to ensure thread-safe when reading/writing of the data

	commands of shell:
	in N X, create X thread each add one to visitor of museum N
	out N X, create X thread each subtract one to visitor of museum N
	info, call museum_info()
	start, create the garbage collector thread that check if a museum have zero vistor
	then it will close that museum
	exit, call museum_clean() and exit the program


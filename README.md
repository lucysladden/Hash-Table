# Hash-Table
Here is an implementation of a hash table data structure i completed in a team with two others. 


This repository also contains a main class in asgn1.c which takes in inputs from a user to compare the performance of different hashing methods. 

## Description of what program does:
The program works by creating a new empty hash table with a default size of 113. It reads input from stdin, converting the input values to indices that can be inserted into the hash table via linear probing (by default) or double hashing (given argument -d).  It dynamically allocates and reallocates memory as required by the size of the input. The program takes in command line arguments from the user to perform various actions. These actions are as follows:


-d : Use double hashing as the collision resolution strategy. (linear probing is the default).
-e : Display entire contents of the hash table on stderr using format string to display the index, frequency, stats and the key.
-p : Print stats info using the function provided in print-stats.txt instead of printing the frequencies of words.
-s snapshots : Display up to the given number of stats snapshots when given -p as an argument. If the table is not full then fewer snapshots will be displayed. Snapshots with 0 entries are not shown.
-t tablesize : Use the first prime >= tablesize as the size of your hash table. Assume that tablesize will be a number greater than 0.
-h : Print a help message describing how to use the program. If error occurs, then show the error, print the help message and exit the program.


The main structure of our program is a switch statement that breaks off into the functions specified above when the corresponding input for that function is received. The default case for when an unknown input is received calls the print_usage function which prints a description of the program so the user can pass an accepted input.

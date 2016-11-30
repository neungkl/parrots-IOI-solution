all:
	gcc -Wall -c grader.cpp -o obj/Debug/grader.o
	gcc -Wall -c student.cpp -o obj/Debug/student.o
	gcc -Wall obj/Debug/grader.o obj/Debug/student.o -o bin/Debug/parrot
	
	
PHONY: all
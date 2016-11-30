all:
	gcc -Wall -c grader.cpp -o obj/Debug/grader.o
	gcc -Wall -c student.cpp -o obj/Debug/student.o
	gcc -Wall obj/Debug/grader.o obj/Debug/student.o -o bin/Debug/parrot

test: all
	echo "1 1" | bin/Debug/parrot
	echo "10 1 2 3 4 5 6 7 8 9 10" | bin/Debug/parrot
	echo "10 255 255 255 255 255 255 255 255 255 255" | bin/Debug/parrot
	echo "20 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255" | bin/Debug/parrot
	echo "64 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255 123 153 111 222" | bin/Debug/parrot
	echo "64 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255" | bin/Debug/parrot
	
PHONY: all test
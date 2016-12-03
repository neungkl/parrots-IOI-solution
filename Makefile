run: all
	@bin/Debug/parrot

all:
	@gcc -Wall -c grader.cpp -o obj/Debug/grader.o
	@gcc -Wall -c student.cpp -o obj/Debug/student.o
	@gcc -Wall obj/Debug/grader.o obj/Debug/student.o -o bin/Debug/parrot

test-real: all
	$(eval export COUNT_BIRD=1)

	@/bin/echo -n "1.1 -> "
	@bin/Debug/parrot < test/subtask1/grader.in.1
	@echo ""

	@/bin/echo -n "2.1 -> "
	@bin/Debug/parrot < test/subtask2/grader.in.1
	@/bin/echo -n "2.2 -> "
	@bin/Debug/parrot < test/subtask2/grader.in.2
	@/bin/echo -n "2.3 -> "
	@bin/Debug/parrot < test/subtask2/grader.in.3
	@/bin/echo -n "2.4 -> "
	@bin/Debug/parrot < test/subtask2/grader.in.4
	@echo ""

	@/bin/echo -n "3.1 -> "
	@bin/Debug/parrot < test/subtask3/grader.in.1
	@/bin/echo -n "3.2 -> "
	@bin/Debug/parrot < test/subtask3/grader.in.2
	@/bin/echo -n "3.3 -> "
	@bin/Debug/parrot < test/subtask3/grader.in.3
	@/bin/echo -n "3.4 -> "
	@bin/Debug/parrot < test/subtask3/grader.in.4
	@echo ""

	@/bin/echo -n "4.1 -> "
	@bin/Debug/parrot < test/subtask4/grader.in.1
	@/bin/echo -n "4.2 -> "
	@bin/Debug/parrot < test/subtask4/grader.in.2
	@/bin/echo -n "4.3 -> "
	@bin/Debug/parrot < test/subtask4/grader.in.3
	@/bin/echo -n "4.4 -> "
	@bin/Debug/parrot < test/subtask4/grader.in.4
	@echo ""
	
	@/bin/echo -n "5.1 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.1
	@/bin/echo -n "5.2 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.2
	@/bin/echo -n "5.3 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.3
	@/bin/echo -n "5.4 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.4
	@/bin/echo -n "5.5 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.6
	@/bin/echo -n "5.6 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.5
	@/bin/echo -n "5.7 -> "
	@bin/Debug/parrot < test/subtask5/grader.in.7

pre-verbose:
	$(eval export VERBOSE=1)

verbose: pre-verbose test

test: all
	@echo "1 1" | bin/Debug/parrot
	@echo "10 1 2 3 4 5 6 7 8 9 10" | bin/Debug/parrot
	@echo "10 255 255 255 255 255 255 255 255 255 255" | bin/Debug/parrot
	@echo "20 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255" | bin/Debug/parrot
	@echo "64 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255 0 128 250 10 9 6 7 100 255 100 0 9 8 20 5 77 98 77 100 255 123 153 111 222" | bin/Debug/parrot
	@echo "64 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255 255" | bin/Debug/parrot

PHONY: all test verbose

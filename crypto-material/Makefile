# Part 2b Makefile

CFLAGS=-Wall -Wextra -Werror -Wcast-qual -Wconversion -Wsign-conversion
CC=gcc

PHONY=default clean python c

default:
	@echo "Please choose whether you want to use C or Python"
	@echo "To this end, please type make c or make python"

c: compute-2b
	./compute-2b

python: compute-2b.py
	./compute-2b.py


clean:
	rm -f *.o *.pyc compute-2b

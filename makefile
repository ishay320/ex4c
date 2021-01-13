CC = gcc -g -Wall -ggdb3
MO = frequency.c


all: frequency

frequency: $(MO)
	$(CC) $^ -o $@

.PHONY: all clear
clear:
	-rm frequency
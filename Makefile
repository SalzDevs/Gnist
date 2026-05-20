.PHONY: all clean run

all: gnist

gnist: main.c
	$(CC) -std=c11 -Wall -Wextra -pedantic -O2 -o $@ main.c

run: gnist
	./gnist

clean:
	rm -f gnist

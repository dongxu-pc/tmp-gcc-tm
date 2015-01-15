CC=g++
CFLAGS=-Wall -std=c++11 -pthread -fgnu-tm

all:	test.out

test.out:	test.cc tm-list.h
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) *.out

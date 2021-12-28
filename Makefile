CC = gcc
CFLAGS = -g -Wall -lm
OBS = parser.o basic.o manip.o
HEAD = parser.h basic.h manip.h const.h
SRC = parser.c basic.c manip.c

parser: $(OBS) $(HEAD) $(SRC)
	$(CC) $(CFLAGS) -o parser $(OBS)

parser.o: $(SRC) $(HEAD) basic.o manip.o
	$(CC) $(CFLAGS) -c parser.c 

manip.o: $(SRC) $(HEAD) basic.o
	$(CC) $(CFLAGS) -c manip.c

basic.o: $(SRC) $(HEAD)
	$(CC) $(CFLAGS) -c basic.c 

clean:
	rm -f *.o parser



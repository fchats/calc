CC = gcc
CFLAGS = -g -Wall -lm
OBS = parser.o basic.o manip.o
HEAD = parser.h basic.h manip.h const.h
SRC = parser.c basic.c manip.c
B = ./build/

$(B)parser: $(OBS) $(HEAD) $(SRC)
	$(CC) $(CFLAGS) -o $(B)parser $(OBS)
	mv *.o ./build
	trash parser
	ln -s ./build/parser ./parser

$(B)parser.o: $(SRC) $(HEAD) ./build/basic.o ./build/manip.o
	$(CC) $(CFLAGS) -c parser.c 

$(B)manip.o: $(SRC) $(HEAD) ./build/basic.o
	$(CC) $(CFLAGS) -c manip.c

$(B)basic.o: $(SRC) $(HEAD)
	$(CC) $(CFLAGS) -c basic.c 

clean:
	rm -f ./build/* ./parser



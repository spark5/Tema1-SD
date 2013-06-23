CC = gcc
CCFLAGS = -Wall
SRC = tema1.c
PROGRAM = lf


all:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)


.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~



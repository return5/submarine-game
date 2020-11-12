CFLAGS = -std=gnu99 -Wall -Wextra -O2 
LIBFLAGS = -lncurses -lm  -I
CC = gcc
SRC = ./src/
HEADERS = headers
OUTFILE =submarine_game 

all:
	$(CC) $(CFLAGS) -o$(OUTFILE) $(SRC)*.c $(LIBFLAGS)$(HEADERS) 

clean:
	rm -f *.o *.gch

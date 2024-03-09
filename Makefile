CC = gcc
CFLAGS = -O2 -g -Wall

OBJS := main.o 

all: main

main: $(OBJS)
		$(CC) -o $@ $(CFLAGS) -c -MMD -MF .$$.d $<

clean: 
		rm -f $(OBJS)
		rm -f *.dSYM
		rm -f main
		rm -f .*.o.d

CC = g++
CFLAGS = -g -Wall
RM = rm -rf
LDLIBS = -lSDL2 -lSDL2_ttf
PREFIX = /usr/local

tme: tme.o stm.o
	$(CC) $(CFLAGS) -o tme tme.o stm.o $(LDLIBS)

stm.o : stm.cc stm.h
	$(CC) -c stm.cc

tme.o : tme.cc stm.h
	$(CC) -c tme.cc

clean:
	$(RM) tme tme.o stm.o


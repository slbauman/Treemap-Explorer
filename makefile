CC = g++
CFLAGS = -g -Wall
RM = rm -rf
LDLIBS = -lSDL2 -lSDL2_ttf

tme: tme.o stm.o scan_dir.o
	$(CC) $(CFLAGS) -o tme tme.o stm.o scan_dir.o $(LDLIBS)

stm.o : stm.cc stm.h
	$(CC) -c stm.cc

tme.o : tme.cc stm.h scan_dir.h
	$(CC) -c tme.cc

scan_dir.o : scan_dir.cc scan_dir.h
	$(CC) -c scan_dir.cc

clean:
	$(RM) tme tme.o stm.o scan_dir.o


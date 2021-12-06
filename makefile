CC = g++
CFLAGS = -g -Wall
TARGET = stm
RM = rm -rf
LDLIBS = -lSDL2
PREFIX = /usr/local

.PHONY: all

$(TARGET): $(TARGET).cc
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cc $(LDLIBS)

clean:
	$(RM) $(TARGET)

.PHONY: install
install: stm
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/stm

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/stm


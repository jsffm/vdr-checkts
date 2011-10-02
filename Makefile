SOURCES=vdr-checkts.cc tsstreamerrorcounter.cc
OBJECTS=$(SOURCES:.cc=.o)
EXECUTABLE=vdr-checkts
CC=g++
CFLAGS=-c -Wall

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)

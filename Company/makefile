TARGET=Company
SOURCE=$(TARGET).cpp
EXECUTABLE=$(TARGET).exe
CFLAGS=-std=c++11
CC=g++

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	rm $(EXECUTABLE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)

# Launch
l: all
	./$(EXECUTABLE)


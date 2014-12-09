CC = clang++

EXECUTABLE = lazybee
SOURCES = main.cpp files.cpp bspmap.cpp
HEADERS = main.h files.h bspmap.h

INCLUDE = include
LIB = lib

CFLAGS = -c -I $(INCLUDE) -Wall -std=c++11
LDFLAGS = -L $(LIB)

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo ""
	@echo "*** Linking objects ***"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	@echo "*** Compiling source files ***"
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXECUTABLE)

run: all
	@echo "*** Running ***"
	./$(EXECUTABLE)

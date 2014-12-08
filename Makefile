CC = clang++

EXECUTABLE = engine
SOURCES = main.cpp files.cpp bsploader.cpp
HEADERS = main.h files.h bsploader.h

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

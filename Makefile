CC = clang++

INCPATH = include
LIBPATH = lib
BINPATH = bin

EXECUTABLE = lazybee
LIBS = -lglfw -lGLEW -lGL
TDOGL = tdogl/Bitmap.cpp tdogl/Camera.cpp tdogl/Program.cpp tdogl/Shader.cpp \
	tdogl/Texture.cpp
SOURCES = main.cpp files.cpp bspmap.cpp renderer.cpp $(TDOGL)

CFLAGS = -c -I $(INCPATH) -Wall -std=c++11
LDFLAGS = -L $(LIBPATH) $(LIBS)

OBJECTS=$(SOURCES:.cpp=.o)
EXEFILE=$(BINPATH)/$(EXECUTABLE)

all: $(SOURCES) $(EXEFILE)

$(EXEFILE): $(OBJECTS)
	@echo ""
	@echo "*** Linking objects ***"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	@echo "*** Compiling source files ***"
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(OBJECTS)
	rm $(EXEFILE)

run: all
	@echo "*** Running ***"
	cd $(BINPATH);./$(EXECUTABLE)

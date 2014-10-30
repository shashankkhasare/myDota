CC = g++
EXEC_NAME = mydota
BASIC_C = main.cpp
OPENGL_FLAGS = -lSDL

all		: $(EXEC_NAME)

$(EXEC_NAME)	: $(BASIC_C)
	$(CC) -w -o $(EXEC_NAME) $(BASIC_C) $(OPENGL_FLAGS)

clean:
	rm $(EXEC_NAME)

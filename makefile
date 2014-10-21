CC = gcc
EXEC_NAME = myDota
BASIC_C = test.c 
OPENGL_FLAGS = -lglut -lGLU -lGL -lm

all		: $(EXEC_NAME)

$(EXEC_NAME)	: $(BASIC_C)
	$(CC) -w -o $(EXEC_NAME) $(BASIC_C) $(OPENGL_FLAGS)

clean:
	rm $(EXEC_NAME)

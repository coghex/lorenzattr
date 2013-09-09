UNAME := $(shell uname)

CC=gcc
ifeq ($(UNAME), Linux)
	LDFLAGS=-lglut -lGLU -lGL -lm
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS=-framework OpenGL -framework GLUT -Wall
endif

.c: ; $(CC) $@.c -o $@.o $(LDFLAGS)

clean: ; rm -f *.o

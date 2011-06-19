NAME = solver_test
COMPILER = g++
GTKFLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTKLIBS = $(shell pkg-config --libs gtk+-2.0)
FLAGS =-std=c++0x -I./ -I../include -L../lib/gsl -lgsl -lgslcblas $(GTKFLAGS) $(GTKLIBS)

.PHONY:all

all:$(NAME)

%:%.cpp
	$(COMPILER) $(FLAGS) $< -o $@

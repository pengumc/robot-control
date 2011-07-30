NAME = solver_test
COMPILER = g++
GTKFLAGS = $(shell pkg-config --cflags gtk+-2.0)
GTKLIBS = $(shell pkg-config --libs gtk+-2.0)
FLAGS =-std=c++0x -I./ -I../include -L../lib/gsl -lgsl -lgslcblas $(GTKFLAGS) $(GTKLIBS)
OBJECTS = $(NAME).o CSolver_new.o rotation.o

.PHONY:all

all:$(NAME)

$(NAME):$(OBJECTS)
	$(COMPILER) -o $(NAME) $(OBJECTS) $(FLAGS)
	rm $(OBJECTS)  

%.o:%.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

%.o:%.c
	$(COMPILER) $(FLAGS) -c $< -o $@



NAME = CServo
COMPILER = g++
FLAGS =-std=c++0x -I../include -L../lib -lgsl -lgslcblas

.PHONY:all

all:$(NAME)

%:%.cpp
	$(COMPILER) $(FLAGS) $< -o $@

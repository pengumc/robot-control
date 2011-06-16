#Author: Michiel van der Coelen
#date: 2011-6-11
#tabsize: 4


NAME = robot-control
GTKFLAGS = $(shell pkg-config --cflags gtk+-2.0)
CFLAGS = -Iinclude -Isrc -I. $(GTKFLAGS)
CPPFLAGS = $(CFLAGS) -std=c++0x  
USBLIBS := $(shell libusb-config --libs)
GTKLIBS = $(shell pkg-config --libs gtk+-2.0)
LIBS =  -Llib/gsl -Llib/robot-control -lgslcblas -lgsl $(LINUX) #put either LINUX or WINDOWS here
LINUX = -lpthread $(USBLIBS) $(GTKLIBS)
WINDOWS = -lusb 
COMPILER = g++
CC = gcc
OUTPUTNAME = $(NAME) 
OBJECTS = opendevice.o CSolver.o CPSController.o CServo.o CUsbDevice.o CGtk.o CQPed.o CAngle.o rotation.o main.o


.PHONY:all, clean, force

all:bin/$(OUTPUTNAME)
force:clean all

bin/$(OUTPUTNAME):$(addprefix lib/$(NAME)/,$(OBJECTS))
	$(COMPILER) -o bin/$(OUTPUTNAME) $(addprefix lib/$(NAME)/,$(OBJECTS)) $(LIBS)
clean:
	rm $(addprefix lib/$(NAME)/,$(OBJECTS))
	rm bin/$(OUTPUTNAME)

lib/$(NAME)/%.o:src/%.cpp
	$(COMPILER) $(CPPFLAGS) -c $< -o $@

lib/$(NAME)/%.o:src/%.c
	$(COMPILER) $(CPPFLAGS) -c $< -o $@

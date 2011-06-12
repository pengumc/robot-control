#Author: Michiel van der Coelen
#date: 2011-6-11
#tabsize: 4


NAME = main
GTKFLAGS = $(shell pkg-config --cflags gtk+-2.0)
CFLAGS = -Iinclude -Isrc -I. $(GTKFLAGS)
CPPFLAGS = $(CFLAGS) -std=c++0x  
USBLIBS := $(shell libusb-config --libs)
GTKLIBS = $(shell pkg-config --libs gtk+-2.0)
LIBS =  -Llib -lgslcblas -lgsl $(LINUX) #put either LINUX or WINDOWS here
LINUX = -lpthread $(USBLIBS) $(GTKLIBS)
WINDOWS = -lusb 
COMPILER = g++
CC = gcc
OUTPUTNAME = $(NAME) 
OBJECTS = opendevice.o CSolver.o CPSController.o CServo.o CUsbDevice.o CGtk.o CQPed.o CAngle.o $(NAME).o


.PHONY:all, clean, force

all:bin/$(OUTPUTNAME)
	rm $(OBJECTS)
force:clean all

bin/$(OUTPUTNAME):$(OBJECTS)
	$(COMPILER) -o bin/$(OUTPUTNAME) $(OBJECTS) $(LIBS)
clean:
	rm $(OBJECTS)
	rm bin/$(OUTPUTNAME)

%.o:src/%.cpp
	$(COMPILER) $(CPPFLAGS) -c $<

%.o:src/%.c
	$(COMPILER) $(CPPFLAGS) -c $<

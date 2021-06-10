# Makefile for PDC homework code exercises

.SUFFIXES:  .o .cpp

CFLAGS := -std=c++11 -Wall 

.cpp.o:
	g++ -c $(CFLAGS) $*.cpp

current:  checkout

CHECKOUT_OBJS = checkout.o
checkout:  $(CHECKOUT_OBJS)
	g++ -o $@ $(CHECKOUT_OBJS)

checkout.o:  checkout.cpp Register.h


-include Makefile.rab

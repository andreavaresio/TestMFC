SHELL=/bin/bash

TARGET = TestMFC.ex

#lista directory in cui cercare sorgenti 
VPATH =	. \

#lista directory in cui cercare .h
INCLUDE = 

#elenco dei sorgenti
SOURCES =  TestMFC.cpp


#elenco lib
LIBS = \
       -lc \
       -lpthread \
       -lstdc++ \
       -ldl \

#dir base di output
BASEOUTDIR = linuxout/

#dipendenza da altre librerie: per ognuna delle librerie aggiungere un comando al target lib
LIBRARYDEPENDENCY = 

include target-linux.mk

lib:
	@echo

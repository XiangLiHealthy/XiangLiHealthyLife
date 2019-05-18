DEBUG ?= 1

DSWITCH = 
ifeq (DEBUG,1)
	$(DSWITCH) = -g 
endif 

SRCDIR = ./thread/
LIBDIR = ./lib/  

all:thread.a

thread.a:
	ar -g -o $(LIBDIR)thread.a $(SRCDIR)*.cpp 

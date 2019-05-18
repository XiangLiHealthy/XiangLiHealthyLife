DEBUG ?= 1

DSWITC = 
ifeq (DEBUG, 1)
	$(DSWITCH) = -g
endif 

SRCDIR = ./business/
LIBDIR = ./lib/ 

all:business.a


business.a:
	echo "生成business.a"
	ar -g -o $(LIBDIR)business.a $(SRCDIR)*.cpp 


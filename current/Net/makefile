bin_path = ./../build/bin/
obj_path = ./../build/obj/
src = ./Net/lib/*.cpp
ser = ./Net/test_server.cpp 
cli = ./Met/test_client.cpp

DEBUG ?= 1

DSWITCH = #默认发布版本
ifeq (DEBUG, 1)
	DSWITCH = -g
endif

all : server.out client.out net.a

net.a : $(ser) $(src)
	#g++ -c  $(src)
	ar -crv  net.a $(src)

server.out : $(ser) $(src)
	g++ -g -o ser.out  $(ser) $(src)

client.out : $(cli) $(src)
	g++ -g -o cli.out  $(cli) $(src)

.PHONY : clean
clean:
	-rm -f *.out *.o *.a 
	

ifeq ($(DEBUG_SYMBOLS),TRUE)
	BUILDDIR = ./build/debug 
else
	BUILDDIR = ./build/release 
endif

LIB = ./lib/

all : $(BUILDDIR) server

$(BUILDDIR):
	@echo "   Create director $@ ..."
	mkdir -p $(BUILDDIR)/bin $(BUILDDIR)/lib


#创建子目录:
#include test.makefile

#编译服务器
#cd ./Net && you-operation-cmds
#include ./Net/makefile
#cd ../ && you-operation-cmds

#编译线程模块为thread.a
#include ./thread/makefile 

#编译业务模块为business.a
#include ./business/makefile 

server : ./thread/*.h
	g++ -g server.cpp ./thread/*.cpp ./net/lib/*.cpp ./business/*.cpp ./DataBase/*.cpp ./log/*.cpp   `mysql_config --cflags --libs` -lpthread -L  $(LIB)*.a $(LIB)*.so  -o server

clean:
	rm -r server
#net.a:
#	ar -o /lib/net.a ./Net/*.cpp

#thread.a:
#	ar -o /lib/thread.a /thread/*.cpp 

#business.a:
#	ar -o /lib/business.a /business/*.cpp

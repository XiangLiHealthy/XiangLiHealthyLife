EXE_NAME = client_tool

LIB = ../../lib/

all: ./*.h ../../include/json/*.h
	g++ -g ./*.cpp ../../thread/*.cpp ../../net/lib/*.cpp ../../sequence/*.cpp  ../../business/*.cpp ../../DataBase/*.cpp ../../log/*.cpp   `mysql_config --cflags --libs` -lpthread -L  $(LIB)*.a $(LIB)*.so  -o $(EXE_NAME)

clean:
	rm -f ./$(EXE_NAME)
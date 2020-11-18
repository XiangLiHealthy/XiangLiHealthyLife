EXE_NAME = client_tool

LIB_DIR = ../../lib
LIB = -L $(LIB_DIR)/*.a $(LIB_DIR)/*.so `mysql_config --cflags --libs` -lpthread

INC = ./*.h ../../include/json/*.h
SRC = ./*.cpp ../../thread/*.cpp ../../net/lib/*.cpp ../../sequence/*.cpp  ../../business/*.cpp ../../DataBase/*.cpp ../../log/*.cpp ../../config/*.cpp
all: $(INC)
	g++ -g $(SRC) $(LIB)  -o $(EXE_NAME)

clean:
	rm -f ./$(EXE_NAME)

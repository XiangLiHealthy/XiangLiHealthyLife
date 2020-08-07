BIN_DIR=/home/xiangli/


cd $BIN_DIR

#./server

ulimit -n 100000
setsid ./server > /dev/null 2 >&1&

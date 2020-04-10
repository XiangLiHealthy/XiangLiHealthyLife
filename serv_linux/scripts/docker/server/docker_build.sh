TAG=xiangli
VER=v1.0.0
BUILD_PATH=../../../

function para_check()
{
	if [ $# -eq 0 ]; then
		echo "use default tag:ver=$TAG:$VER"
		return 0
	fi

	if [ $# -ne 2 ]; then
		echo "you must input two para,eg:xiangli:v1.0.0"
		return 1
	fi

	TAG=$1
	VER=$2
	echo "tag:ver=$TAG:$VER"

	return 0
}

function compile()
{
	WK=`pwd`

	cd ../../../

	make clean
	make
	if [ $? -ne 0 ];then
		echo "make failed"
		return -1
	fi
	
	cd $WK

	return 0
}

function copy_file()
{
	cp ../../../server ./
	if [ $? -ne 0 ];then
		return 1
	fi
	
	rm -r spider

	cp -r ../../../../spider ./
	if [ $? -ne 0 ]; then
		echo "copy spider failed"
		return 1
	fi

	return 0
}

function build_img()
{
	docker build -t $TAG:$VER .
}

function push_img()
{
	echo "push img ..."
	
	return 0
}

function main()
{
	para_check
	if [ $? -ne 0 ]; then
		echo "para check failed"
		return 1
	fi

	compile
	if [ $? -ne 0 ]; then
		echo "compile failed"
		return 1
	fi

	copy_file
	if [ $? -ne 0 ]; then
		echo "copy file failed"
		return 1
	fi
	
	build_img
	if [ $? -ne 0 ]; then
		echo "build image failed"
		return 1
	fi

	push_img
	if [ $? -ne 0 ]; then
		echo "push image failed"
		return 1
	fi

	return 0
}

main


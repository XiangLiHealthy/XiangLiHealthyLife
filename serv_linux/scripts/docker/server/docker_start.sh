CONTAINER=xiangli
TAG=xiangli
VER=v1.0.0

function parser_para()
{
	if [ $# -eq 0 ]; then
		echo "use default tag:ver=$TAG:$VER"
		return 0
	fi

	if [ $# -ne 2 ]; then
		echo "para format wrong,eg:xiangli v1.0.0"
		return 1
	fi

	TAG=$1
	VER=$2

	echo "tag:ver=$TAG:$VER"
	return 0
}

function stop_docker()
{
	RUN_CONTAINER=`docker ps |grep -w $CONTAINER`
	if [ -n "$RUN_CONTAINER" ];then
		echo "stop docker $RUN_CONTAINER"
		docker stop $CONTAINER
		return $?
	fi

	return 0
}

function start_docker()
{
	docker run -itd --privileged --name xiangli --rm --network=host -v /home/ubuntu/XiangLiHealthyLife/serv_linux/:/home/xiangli/src $TAG:$VER /bin/bash
	if [ $? -ne 0 ]; then
		echo "run docker failed"
		return 1
	fi

	return 0
}

function dev_into_docker()
{
  xhost +local:root 1>/dev/null 2>&1
  docker exec \
    -i $CONTAINER \
    /bin/bash
  xhost -local:root 1>/dev/null 2>&1
}

function start_server()
{
	dev_into_docker < start_modules.sh 
	if [ $? -ne 0 ]; then
	       echo "start server failed"
		return 1
	fi

	return 0	
}

function main()
{
	parser_para
	if [ $? -ne 0 ]; then
		echo "para is wrong"
		return 1
	fi

	stop_docker
	if [ $? -ne 0 ]; then
		echo "stop docker failed"
		return 1
	fi

	start_docker
	if [ $? -ne 0 ]; then
		echo "start docker failed"
		return 1
	fi
	
	start_server
	if [ $? -ne 0 ]; then
		echo "start service failed"
		return 1
	fi

	return 0
}

main


CONTAINER=mysql
TAG=xiangli_mysql
VER=v1.0.0
CONFIG=/etc/mysql
DATA=/var/lib/mysql

echo $DOCKER

#停止容器
function stop_container()
{
	CONTAINERS=`docker ps | grep $CONTAINER`

	if [ -n "$CONTAINERS" ]; then
		docker stop $CONTAINER

	fi
}

#启动容器
function start_container()
{

	docker run -itd --name $CONTAINER --rm \
		-v /etc/mysql:/etc/mysql \
		-v /var/lib/mysql:/etc/lib/mysql \
		-p 3306:3306 \
		--privileged=true \
		$TAG:$VER bash
}

function copy_data()
{
	echo "cp $CONTAINER:$1 ---> $1"

	#启动容器
	docker run -itd --name $CONTAINER --rm $TAG:$VER bash 
	if [ $? -ne 0 ]; then
		echo "start container failed"
		return 1
	fi

	#拷贝数据
	sudo docker cp $CONTAINER:$1 $1
	if [ $? -ne 0 ]; then
		echo "cp $1 failed"
		return 1
	fi

	#停止容器
	docker stop $CONTAINER
	if [ $? -ne 0 ]; then
		echo "docker stop failed"
		return 1
	fi

	return 0
}

#如果主机没有mysql的配置和数据就拷贝容器的数据出来
function create_dir()
{
	#/etc/my.cnf is not exist
	if [ ! -d "$CONFIG" ] ; then
		copy_data $CONFIG	
	fi

	#/var/lib/mysql
	if [ ! -d "$DATA" ] ; then
		echo "copy data from container"
		copy_data $DATA
	fi
}


function parser_para()
{
	if [ $# -eq 2 ] ; then
		TAG=$1
		VER=$2
	fi

	echo "IMG=$TAG:$VER"
}

function create_db()
{
	echo "test"	
}

function main()
{
	parser_para

	echo "stop container"
	stop_container
	
	echo "create mysql my.cnf and /var/lib/mysql"
	create_dir
	
	echo "start container"
	start_container

	echo "create db"
	create_db
}

main


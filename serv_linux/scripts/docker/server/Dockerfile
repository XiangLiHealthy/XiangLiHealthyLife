FROM ubuntu:18.04

#替换为阿里源镜像，加快下载
COPY sources.list /etc/apt/

RUN buildDev='vim libmysqlclient-dev wget gdb python3-pip python3-requests python3-bs4' \
	&& apt-get update \
	&& apt-get upgrade -y \
	&& apt-get install $buildDev -y \ 
	&& useradd -m xiangli -p 123456 \
	&& mkdir -p /home/xiangli/lib \
	&& pip3 install pymysql
	#&& apt-get purge -y --auto-remove $buildDev


#复制mysql和json库
COPY libjson_linux-gcc-7_libmt.so /home/xiangli/lib/
COPY server_start.sh /home/xiangli/
COPY server /home/xiangli/
COPY spider /home/xiangli/spider

#支持中文编辑显示
ENV LANG C.UTF-8

#声明mysql会使用的端口
EXPOSE 8888

#自动启动mysql
#ENTRYPOINT bash /home/xiangli/server_start.sh

CMD ["/bin/bash"]

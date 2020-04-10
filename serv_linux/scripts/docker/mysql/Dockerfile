FROM ubuntu:18.04

#替换为阿里源镜像，加快下载
COPY sources.list /etc/apt/

#添加mysql启动脚本
COPY mysql_start.sh /home/

RUN buildDev='mysql-server mysql-client vim' \
	&& apt-get update \
	&& apt-get upgrade -y \
	&& apt-get install $buildDev -y \
	&& apt-get autoclean -y \
	&& apt-get clean -y \
	&& apt-get autoremove -y \
	&& useradd -m xiangli -p 123456


#编辑器支持中文
ENV LANG C.UTF-8

#声明mysql会使用的端口
EXPOSE 3306

#自动启动mysql
ENTRYPOINT bash /home/mysql_start.sh

CMD ["/bin/bash"]

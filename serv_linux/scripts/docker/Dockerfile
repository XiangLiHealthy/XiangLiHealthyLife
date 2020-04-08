FROM ubuntu:18.04

#替换为阿里源镜像，加快下载
COPY sources.list /etc/apt/

RUN apt-get update && \
		   apt-get upgrade -y && \
		   apt-get install build-essential -y&&\
		   apt-get install mysql-server -y &&\
		   apt-get install mysql-client -y

#声明mysql会使用的端口
EXPOSE 3306

CMD ["/bin/bash"]

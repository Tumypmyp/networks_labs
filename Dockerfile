FROM ubuntu:latest

WORKDIR /home

RUN mkdir user

RUN apt-get update --fix-missing

RUN apt-get -y --force-yes install vim git 

RUN apt-get -y --force-yes install build-essential

RUN apt-get -y --force-yes install net-tools iputils-ping

RUN apt-get -y --force-yes install tree

ENTRYPOINT /bin/bash
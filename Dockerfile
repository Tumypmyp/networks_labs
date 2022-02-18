FROM ubuntu:latest

WORKDIR /home

RUN mkdir tumypmyp

RUN apt-get update --fix-missing

RUN apt-get -y --force-yes install vim git 

RUN apt-get -y --force-yes install build-essential

RUN apt-get -y --force-yes install net-tools iputils-ping

RUN apt-get -y --force-yes install tree


ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -y --force-yes install tshark

WORKDIR /home/tumypmyp

RUN git clone https://github.com/tumypmyp/networks_labs/

ENTRYPOINT /bin/bash

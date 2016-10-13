FROM ubuntu:16.04
MAINTAINER Tomas Krcka <tomas.krcka@relayr.de>
RUN apt-get -y update
RUN apt-get -y install software-properties-common python-software-properties
RUN apt-get -y remove binutils-arm-none-eabi gcc-arm-none-eabi

RUN apt-get -y install gcc-arm-none-eabi make
RUN apt-get -y install openssh-client sshpass
RUN apt-get -y install git unzip

COPY compile_test.sh /root/
RUN chmod +x /root/compile_test.sh

ENV HOMEDIR /home/xdk
ENV XDK_MQTT /home/xdk/xdk-mqtt
ENV XDK_SDK /home/xdk/SDK

RUN mkdir -p $HOMEDIR
RUN mkdir -p $XDK_SDK

## SDK from external directory
ADD xdk110 $XDK_SDK/xdk110

ENTRYPOINT ["/root/compile_test.sh"]

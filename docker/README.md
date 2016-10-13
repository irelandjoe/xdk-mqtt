# Docker container for compiling XDK

## 1. Introduction

Docker container can be used for nightly builds or for checking of compilation.


## 2. Prerequisities

### 2.1. List of components

-  [Docker](https://docs.docker.com/engine/installation) 
-  [XDK Workbench release 1.6.0](http://xdk.bosch-connectivity.com/software-downloads)


## 3. Environment preparation

1.  Install [XDK Workbench to Windows](xdk-onboarding.md)
2.  Go to C:\XDK-Workbench\SDK\ and copy `xdk110` folder to docker directory

## 4. Docker container
1.  Go to docker directory where `Dockerfile` is.
2.  Create a container: `$ docker build -t="xdk-mqtt" .`
3.  Start the container: `$ docker run -i -t xdk-mqtt`



# XDK developing in Linux/MacOS environment

## 1. Introduction

This document contains instructions how to compile XDK in Linux and MacOS.
It's for Linux and MacOS skilled users.

## 2. Prerequisities

### 2.1. List of components

-  XDK Workbench version 1.7.0 (version 1.6.0 is also supported) 
-  Segger JLinkExe - https://www.segger.com/jlink-software.html (JLinkExe in PATH variable)
-  ARM GCC - https://launchpad.net/gcc-arm-embedded/4.9/4.9-2015-q3-update
-  debug probe (currently supported: JTAG Lock Pick Tiny 2 via OpenOCD and Segger JLink)

## 3. Environment preparation

1.  Install [XDK Workbench to Windows](xdk-onboarding.md)
2.  Go to C:\XDK-Workbench\SDK\ and copy `xdk110` folder to your Linux or MacOS (e.g. to /opt directory)
3.  Set XDK_SDK environment variable to XDK SDK root directory (the one containing xdk110 folder)
4.  Set GCC_ARM environment variable to ARM gcc compiler installation
5.  Set SHELLNAMES and SH variables in \<XDK_SDK\>/xdk110/make/application.mk to your current shell (bash, zsh, etc.)
6.  Make sure that both GCC ARM and Segger JLink/OpenOCD binaries are available from command line

## 4. Compile the firmware

1.  Go to \<THIS_APP_REPO\>
2.  Connect XDK with USB cable (and debug probe on Linux/OSX) and turn it on
3.  Modify credentials and WiFi SSID - [see section 6](xdk-onboarding.md)
4.  Run: make (or "make flashsegger" or "make flashocd" to build and flash with appropriate debugger)

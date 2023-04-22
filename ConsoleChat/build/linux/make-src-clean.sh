#!/bin/bash
#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

PROJECT_ROOT=""

if [ -d "../../temp" ] && [ -d "../../src" ]; then
	PROJECT_ROOT="../.."
elif [ -d "./temp" ] && [ -d "./src" ]; then
	PROJECT_ROOT="."
fi

#
# libraries
#
if [ -d "$PROJECT_ROOT/lib" ]; then
	rm -f $PROJECT_ROOT/lib/libnetdrone_ccu30.a
fi

#
# temporaries
#
if [ -d "$PROJECT_ROOT/temp" ]; then
	rm -rf $PROJECT_ROOT/temp/*
fi

#
# autogen
#
if [ -d "$PROJECT_ROOT/src/consolechatserver-linux/build" ]; then
	rm -f $PROJECT_ROOT/src/consolechatserver-linux/build/build.h
	rm -f $PROJECT_ROOT/src/consolechatserver-linux/build/cmake_uninstall.cmake
	rm -f $PROJECT_ROOT/src/consolechatserver-linux/build/echoserver.spec
fi

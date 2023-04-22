#!/bin/bash
#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

PROJECT_NAME3="consolechatclient"
PROJECT_NAME4="consolechatserver-dotnet"

PROJECT_ROOT=""

if [ -d "../../temp" ] && [ -d "../../src" ]; then
	PROJECT_ROOT="../.."
elif [ -d "./temp" ] && [ -d "./src" ]; then
	PROJECT_ROOT="."
fi

#
# temporaries
#
if [ -d "$PROJECT_ROOT/temp" ]; then
	rm -rf $PROJECT_ROOT/temp/*
fi

#
# objects
#
if [ -d "$PROJECT_ROOT/src/$PROJECT_NAME3/obj" ]; then
	echo "remove $PROJECT_ROOT/src/$PROJECT_NAME3/obj directory"
	rm -rf $PROJECT_ROOT/src/$PROJECT_NAME3/obj
fi

if [ -d "$PROJECT_ROOT/src/$PROJECT_NAME4/obj" ]; then
	echo "remove $PROJECT_ROOT/src/$PROJECT_NAME4/obj directory"
	rm -rf $PROJECT_ROOT/src/$PROJECT_NAME4/obj
fi

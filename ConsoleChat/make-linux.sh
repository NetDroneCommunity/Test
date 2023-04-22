#!/bin/bash
#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

OS_TYPE=''
BUILD_TYPE='Debug'
INSTALL_PREFIX='/opt/local'
ECLIPSE='false'

#
# /etc/os-release
#
OS_RELEASE_ID=`cat /etc/os-release | grep 'ID=' | grep -v 'VERSION_ID=' | grep -v 'PLATFORM_ID=' | grep -v 'VARIANT_ID=' | sed -e 's/ID=//g' | sed -e 's/\"//g'`

if [ "$OS_RELEASE_ID" = "gentoo" ]; then
	OS_TYPE="Gentoo"
elif [ "$OS_RELEASE_ID" = "centos" ]; then
	OS_RELEASE_VERSION_ID=`cat /etc/os-release | grep 'VERSION_ID=' | sed -e 's/VERSION_ID=//g' | sed -e 's/\"//g'`
	OS_TYPE="CentOS$OS_RELEASE_VERSION_ID"
elif [ "$OS_RELEASE_ID" = "ubuntu" ]; then
	OS_RELEASE_VERSION_ID=`cat /etc/os-release | grep 'VERSION_ID=' | sed -e 's/VERSION_ID=//g' | sed -e 's/\"//g' | cut -f1 -d '.'`
	OS_TYPE="Ubuntu$OS_RELEASE_VERSION_ID"
elif [ "$OS_RELEASE_ID" = "rhel" ]; then
	OS_RELEASE_VERSION_ID=`cat /etc/os-release | grep 'VERSION_ID=' | sed -e 's/VERSION_ID=//g' | sed -e 's/\"//g' | cut -f1 -d '.'`
	OS_TYPE="RHEL$OS_RELEASE_VERSION_ID"
elif [ "$OS_RELEASE_ID" = "ol" ]; then
	OS_RELEASE_VERSION_ID=`cat /etc/os-release | grep 'VERSION_ID=' | sed -e 's/VERSION_ID=//g' | sed -e 's/\"//g' | cut -f1 -d '.'`
	OS_TYPE="Oracle$OS_RELEASE_VERSION_ID"
else
	OS_TYPE="$OS_RELEASE_ID"
fi

#echo "params: 1:$1, 2:$2, 3:$3, 4:$4"

if [ "$1" = "Debug_ConsoleChatServer" ]; then
	BUILD_TYPE='Debug'
elif [ "$1" = "Release_ConsoleChatServer" ]; then
	BUILD_TYPE='Release'
elif [ "$1" = "Debug_Eclipse_ConsoleChatServer" ]; then
	BUILD_TYPE='Debug'
	ECLIPSE='true'
elif [ "$1" = "Release_Eclipse_ConsoleChatServer" ]; then
	BUILD_TYPE='Release'
	ECLIPSE='true'
else
	BUILD_TYPE='All'
fi

if [ ! "$2" = "" ]; then
	INSTALL_PREFIX=$2
fi

if [ "$ECLIPSE" = "true" ]; then
	workspace=''
	
	if [ ! "$3" = "" ]; then
		workspace=$3
	fi

	if [ -d "$workspace/lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}" ]; then
		cp -fv "$workspace/lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}/libnetdrone_ccu30.a" "$workspace/lib/libnetdrone_ccu30.a"
	fi
	
	if [ ! -d "$workspace/temp/${BUILD_TYPE}_ConsoleChatServer" ]; then
		mkdir -p "$workspace/temp/${BUILD_TYPE}_ConsoleChatServer"
	fi
	
	pushd "$workspace/temp/${BUILD_TYPE}_ConsoleChatServer"
	cmake ../../src/consolechatserver-linux/build -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_ARCH=x86_64 -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX

	if [ "$4" = "all" ]; then
		make install
	else
		make $4
	fi
	popd
else
	sh ./build/linux/make-src-clean.sh

	if [ "$BUILD_TYPE" = "All" ]; then
		#
		# Debug
		#
		BUILD_TYPE='Debug'
		if [ -d "lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}" ]; then
			cp -fv "lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}/libnetdrone_ccu30.a" "lib/libnetdrone_ccu30.a"
		fi
		
		if [ ! -d "temp/${BUILD_TYPE}_ConsoleChatServer" ]; then
			mkdir -p "temp/${BUILD_TYPE}_ConsoleChatServer"
		fi
		
		pushd "temp/${BUILD_TYPE}_ConsoleChatServer"
		cmake ../../src/consolechatserver-linux/build -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_ARCH=x86_64 -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX
		make $3
		popd

		#
		# Release
		#
		BUILD_TYPE='Release'
		if [ -d "lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}" ]; then
			cp -fv "lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}/libnetdrone_ccu30.a" "lib/libnetdrone_ccu30.a"
		fi
		
		if [ ! -d "temp/${BUILD_TYPE}_ConsoleChatServer" ]; then
			mkdir -p "temp/${BUILD_TYPE}_ConsoleChatServer"
		fi

		pushd "temp/${BUILD_TYPE}_ConsoleChatServer"
		cmake ../../src/consolechatserver-linux/build -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_ARCH=x86_64 -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX
		make $3
		popd
	else
		if [ -d "lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}" ]; then
			cp -fv "lib/${BUILD_TYPE}_NetDrone_${OS_TYPE}/libnetdrone_ccu30.a" "lib/libnetdrone_ccu30.a"
		fi
		
		if [ ! -d "temp/${BUILD_TYPE}_ConsoleChatServer" ]; then
			mkdir -p "temp/${BUILD_TYPE}_ConsoleChatServer"
		fi
		
		pushd "temp/${BUILD_TYPE}_ConsoleChatServer"
		cmake ../../src/consolechatserver-linux/build -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_ARCH=x86_64 -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX
		make $3
		popd
	fi
fi

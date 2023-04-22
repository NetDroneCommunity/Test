#!/bin/bash
#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

proc_option() {
	local optarg=""

	case $1 in
		-*=*)
		optarg=`echo $1 | sed 's/[-_a-zA-Z0-9]*=//'` ;;
	esac

	case $1 in
		--work-path=*)
			if [ ! "$optarg" = "" ]; then
				if [ -d $optarg ]; then
					WORK_DIR=$optarg
				else
					echo "work directory is not found"
				fi
			else
				echo "error"
			fi
			;;
		--build-path=*)
			if [ ! "$optarg" = "" ]; then
				if [ -d $optarg ]; then
					BUILD_DIR=$optarg
				else
					echo "build directory is not found"
				fi
			else
				echo "error"
			fi
			;;
		--include-path=*)
			if [ ! "$optarg" = "" ]; then
				if [ -d $optarg ]; then
					INCLUDE_DIR=$optarg
				else
					echo "include directory is not found"
				fi
			else
				echo "error"
			fi
			;;
		--src-path=*)
			if [ ! "$optarg" = "" ]; then
				if [ -d $optarg ]; then
					SRC_DIR=$optarg
				else
					echo "src directory is not found"
				fi
			else
				echo "error"
			fi
			;;
		--package-name=*)
			if [ ! "$optarg" = "" ]; then
				PACKAGE_NAME=$optarg
			else
				echo "error"
			fi
			;;
	esac
}

proc_create() {
    echo "work dir: $WORK_DIR"
    echo "build dir: $BUILD_DIR"
    echo "include dir: $INCLUDE_DIR"
    echo "src dir: $SRC_DIR"
    echo "package name: $PACKAGE_NAME"

	local build_base=`$CMD_BASENAME $BUILD_DIR`
	local package_base=`echo $PACKAGE_NAME | $CMD_CUT -f1 -d '-'`

	if [ -d "$WORK_DIR/$PACKAGE_NAME" ]; then
		$CMD_RM -rf "$WORK_DIR/$PACKAGE_NAME"
		echo "delete old package directory"
	fi

	if [ -f "$WORK_DIR/$PACKAGE_NAME.tar.gz" ]; then
		$CMD_RM -f "$WORK_DIR/$PACKAGE_NAME.tar.gz"
		echo "delete old package file"
	fi

	$CMD_MKDIR -p "$WORK_DIR/dist/$PACKAGE_NAME"
	if [ ! "$INCLUDE_DIR" = "" ]; then
		$CMD_CP -a "$INCLUDE_DIR" "$WORK_DIR/dist/$PACKAGE_NAME"
	fi
	if [ ! "$SRC_DIR" = "" ]; then
		$CMD_CP -a "$SRC_DIR" "$WORK_DIR/dist/$PACKAGE_NAME"
	fi
	if [ ! "$BUILD_DIR" = "" ]; then
		$CMD_CP -a "$BUILD_DIR" "$WORK_DIR/dist/$PACKAGE_NAME"
	fi

	for s in `find $WORK_DIR/dist/$PACKAGE_NAME -name ".svn"`;
	do
		if [ -d $s ]; then
			rm -rf $s;
		fi
	done

	for s in `find $WORK_DIR/dist/$PACKAGE_NAME -name "Makefile.am"`;
	do
		if [ -f $s ]; then
			rm -f $s;
		fi
	done

	for s in `find $WORK_DIR/dist/$PACKAGE_NAME -name "Makefile.in"`;
	do
		if [ -f $s ]; then
			rm -f $s;
		fi
	done

	$CMD_RM -f "$WORK_DIR/dist/$PACKAGE_NAME/$build_base/cmake_libtool.cmake"
	$CMD_RM -f "$WORK_DIR/dist/$PACKAGE_NAME/$build_base/cmake_uninstall.cmake"
	$CMD_RM -f "$WORK_DIR/dist/$PACKAGE_NAME/$build_base/$package_base.spec"
	$CMD_RM -f "$WORK_DIR/dist/$PACKAGE_NAME/$build_base/config.h"

	pushd $WORK_DIR/dist 1> /dev/null 2>&1
	$CMD_TAR -czf "../$PACKAGE_NAME.tar.gz" "$PACKAGE_NAME"
	#$CMD_RM -rf "$WORK_DIR/dist/$PACKAGE_NAME"
	popd 1> /dev/null 2>&1

	$CMD_CP "$BUILD_DIR/$package_base.spec" "$WORK_DIR/$package_base.spec"

	echo "Complete"
}

CMD_CP='/bin/cp'
CMD_RM='/bin/rm'
CMD_TAR='/bin/tar'
CMD_MKDIR='/bin/mkdir'
CMD_BASENAME='/bin/basename'
CMD_CUT='/bin/cut'

PROG_NAME=`$CMD_BASENAME $0`
WORK_DIR=''
BUILD_DIR=''
INCLUDE_DIR=''
SRC_DIR=''
PACKAGE_NAME=''

if [ ! "$1" = "" ] && [ ! "$2" = "" ] && [ ! "$3" = "" ] && [ ! "$4" = "" ] && [ ! "$5" = "" ]; then
	proc_option $1
	proc_option $2
	proc_option $3
	proc_option $4
	proc_option $5

	if [ ! "$WORK_DIR" = "" ]; then
		if [ ! "$BUILD_DIR" = "" ]; then
			if [ ! "$INCLUDE_DIR" = "" ]; then
				if [ ! "$SRC_DIR" = "" ]; then
					if [ ! "$PACKAGE_NAME" = "" ]; then
						proc_create
					fi
				fi
			fi
		fi
	fi
elif [ ! "$1" = "" ] && [ ! "$2" = "" ] && [ ! "$3" = "" ] && [ ! "$4" = "" ]; then
	proc_option $1
	proc_option $2
	proc_option $3
	proc_option $4

	if [ ! "$WORK_DIR" = "" ]; then
		if [ ! "$BUILD_DIR" = "" ]; then
			if [ ! "$SRC_DIR" = "" ]; then
				if [ ! "$PACKAGE_NAME" = "" ]; then
					proc_create
				fi
			fi
		fi
	fi
else
    cat <<EOF
Usage: $PROG_NAME [OPTIONS]
Options:
	--work-path=[DIRECTORY]
	--build-path=[DIRECTORY]
	--include-path=[DIRECTORY]
	--src-path=[DIRECTORY]
	--package-name=[PACKAGE]
EOF
    exit 1
fi

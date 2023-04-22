#!/bin/bash
#
# NetDrone Engine
# Copyright © 2022 Origin Studio Inc.
#

case $@ in
	-*=*) optarg=`echo $@ | sed 's/[-_a-zA-Z0-9]*=//'` ;;
	*) optarg= ;;
esac

case $@ in
	--all=*)
		if [ ! "$optarg" = "" ]; then
			if [ -f $optarg ]; then
				MAJOR_VERSION=`cat $optarg | \
				grep 'static const INT iSERVICE_MAJOR_VERSION' | \
				sed -e 's/static const INT iSERVICE_MAJOR_VERSION = //g' | \
				sed -e 's/[;\r]//g'`
				MINOR_VERSION=`cat $optarg | \
				grep 'static const INT iSERVICE_MINOR_VERSION' | \
				sed -e 's/static const INT iSERVICE_MINOR_VERSION = //g' | \
				sed -e 's/[;\r]//g'`
				PATCH_VERSION=`cat $optarg | \
				grep 'static const INT iSERVICE_PATCH_VERSION' | \
				sed -e 's/static const INT iSERVICE_PATCH_VERSION = //g' | \
				sed -e 's/[;\r]//g'`
				echo "$MAJOR_VERSION.$MINOR_VERSION.$PATCH_VERSION"
			else
				echo "file not found"
			fi
		else
			echo "error"
		fi
		;;
	--major-version=*)
		if [ ! "$optarg" = "" ]; then
			if [ -f $optarg ]; then
				MAJOR_VERSION=`cat $optarg | \
				grep 'static const INT iSERVICE_MAJOR_VERSION' | \
				sed -e 's/static const INT iSERVICE_MAJOR_VERSION = //g' | \
				sed -e 's/[;\r]//g'`
				echo $MAJOR_VERSION
			else
				echo "file not found"
			fi
		else
			echo "error"
		fi
		;;
	--minor-version=*)
		if [ ! "$optarg" = "" ]; then
			if [ -f $optarg ]; then
				MINOR_VERSION=`cat $optarg | \
				grep 'static const INT iSERVICE_MINOR_VERSION' | \
				sed -e 's/static const INT iSERVICE_MINOR_VERSION = //g' | \
				sed -e 's/[;\r]//g'`
				echo $MINOR_VERSION
			else
				echo "file not found"
			fi
		else
			echo "error"
		fi
		;;
	--patch-version=*)
		if [ ! "$optarg" = "" ]; then
			if [ -f $optarg ]; then
				PATCH_VERSION=`cat $optarg | 
				grep 'static const INT iSERVICE_PATCH_VERSION' | \
				sed -e 's/static const INT iSERVICE_PATCH_VERSION = //g' | \
				sed -e 's/[;\r]//g'`
				echo $PATCH_VERSION
			else
				echo "file not found"
			fi
		else
			echo "error"
		fi
		;;
	*)
		cat <<EOF
Usage: getversion_service.sh [OPTIONS]
Options:
	--all=[FILE]			print all version information
	--major-version=[FILE]	print major version information
	--minor-version=[FILE]	print minor version information
	--patch-version=[FILE]	print patch version information
EOF
	exit 1
	;;
esac


#!/bin/bash

if [ $# -lt 1 ]
then
	#on: mac os line
	#off: linux
        echo "usage: d(r) off(on) nor(cd) wl(nl)"
        exit
fi

mode=$1
platform=$2
ver2=$3
ver_logo=$4
approot=''

if [ $mode = "d" ]
then
	approot=libs-build/debug
elif [ $mode = "r" ]
then
	approot=libs-build/release
else
        echo "usage: d(r)"
        exit
fi

mkdir -p $approot


#mac下，用cp会造成程序重新编译
cpt=cp
if [ $platform = "on" ]; then
	cpt='rsync -av'
fi


$cpt include/version/version2_normal.h include/version/version2.h
if [ $ver2 = "cd" ]; then
	$cpt -av include/version/version2_cd.h include/version/version2.h
fi

cp include/version/version_withlogo.h include/version/version_logo.h
if [ $ver_logo = "nl" ]; then
	cp -av include/version/version_nologo.h include/version/version_logo.h
fi

rsync -av ../../trunk/warning_disable.pri warning_disable.pri
rsync -av --exclude boost/ ../../trunk/include/* include/

# run libs
libdir=''
if [ $platform = "on" ]; then
	dpost="dylib"
	libdir="mac/x64/clang11"
else
        dpost="so"
        libdir="linux/x64/gcc7"
fi
rsync -av --exclude libassistant.a --exclude libDeviceTask.* --exclude *.app ../../trunk/$approot/bin/*.* $approot/bin/

#etc
rsync -av etc/* $approot/

fsdir=''
#filesystem
if [ $platform = "on" ]; then
	fsdir=$approot/SafeImager.app/Contents/dylibs/
else
	fsdir=$approot/bin/
fi
mkdir -p $fsdir
rsync -av lib/$libdir/filesystem/* $fsdir

# lib
mkdir -p lib/$libdir
rsync -av ../../trunk/libs/$libdir/*.a lib/$libdir/
rsync -av ../../trunk/libs/$libdir/*.$dpost lib/$libdir/

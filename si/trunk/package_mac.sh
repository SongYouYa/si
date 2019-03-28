#!/bin/bash

if [ $# -lt 1 ];then
	echo "usage: d(r)"
	exit
fi

mode=$1
modedir=""

if [ $mode = "d" ];then
	modedir='debug'
elif [[ $mode = "r" ]]; then
	modedir='release'
else
	echo 'usage: d(r)'
	exit
fi


libdir=lib/mac/x64/clang11
libbdir=libs-build/$modedir/
apppath=$libbdir/SafeImager.app
ContentsDir=$apppath/Contents
frames_dir=$ContentsDir/Frameworks
eForenPath=$ContentsDir/MacOS/SafeImager


`~/Qt5.9.5/5.9.5/clang_64/bin/macdeployqt $apppath`

function scplib () {
	if [ -f $1 ]; then
		cp -a $1 $frames_dir
	fi
}
function change_rpath () {
	`install_name_tool -change $1 @rpath/$2 $eForenPath`	
}

#libname=libafflib.0.dylib
#change_rpath /usr/local/lib/$libname $libname
#scplib $libdir/$libname

targetname='si_mac_online.tar.bz2'

rm -rf si
mkdir si
# remove last tar
rm -f $targetname

echo 'copying...'

# config
mkdir -p si/conf
cp -a etc/conf si/


# resource
mkdir -p si/resource
mkdir -p si/resource/lang
cp etc/resource/lang/*.qm si/resource/lang
cp -a etc/resource/report si/resource/

# tool
mkdir -p si/tool/video
cp -a etc/tool/video/mac  si/tool/video/
cp -a etc/tool/macpmemdriver si/tool/macpmemdriver

#bin
cp -a $apppath si

echo 'compress......'
tar -cjf $targetname si
echo 'tar finished.'

#readload file
ftpdir='SafeImager/sitar'
read -p "upload the tar?(y/n)" UPLOAD
if [ $UPLOAD == "y" ]
then
        echo "uploading tar..."
        PATH="/usr/local/opt/inetutils/libexec/gnubin:$PATH"
       ./upload_tar.sh $ftpdir $targetname
        echo "upload finished"
fi
echo 'done'

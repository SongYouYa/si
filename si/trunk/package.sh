#!/bin/bash

if [ $# -lt 0 ]
then
	echo "usage: nor(cd) wl(nl)"
	exit
fi

ver2=$1
ver_logo=$2

gccdir=gcc7

targetname='si'
ftpdir='SafeImager/sitar'

if [ $ver2 = "cd" ]
then
	targetname=$targetname'_cd'
fi

if [ $ver_logo = "nl" ]; then
	targetname=$targetname'_nologo'
fi
targetname=$targetname'.tar.bz2'

rm -rf si
mkdir si
# remove last tar
rm -f $targetname

echo 'copying...'

# conf
mkdir -p si/conf
cp -a etc/conf/* si/conf

# resource
mkdir -p si/resource
#cp -r etc/resource/doc si/resource
mkdir -p si/resource/lang
cp etc/resource/lang/*.qm si/resource/lang
cp -a etc/resource/report si/resource/

# tool
mkdir -p si/tool/video
cp -a etc/tool/video/linux  si/tool/video/
mkdir -p si/tool/zip
cp -a etc/tool/zip/7za si/tool/zip/7za

# bin
mkdir -p si/bin

cp libs-build/release/bin/eForen si/bin/eForen
arr=`ldd si/bin/eForen`
for line in $arr
do
        if [ -f $line ];then
#		echo ""
                cp -f $line si/bin
        fi
done

#libs of safeimager
libs=(
libs-build/release/bin/*.so*
lib/linux/x64/$gccdir/lib*.so*
lib/linux/x64/$gccdir/filesystem/*
lib/linux/x64/$gccdir/qt/*
)
for i in ${libs[*]}
do
        cp -rdf $i si/bin
done

echo 'compress......'
tar -cjf $targetname si
echo 'tar finished.'

#readload file
read -p "upload the tar?(y/n)" UPLOAD
if [ $UPLOAD == "y" ]
then
        echo "uploading tar..."
       	bash ./upload_tar.sh $ftpdir $targetname
        echo "upload finished"
fi
echo 'done'

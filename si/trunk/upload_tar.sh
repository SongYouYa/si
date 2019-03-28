#!/bin/bash

if [ $# -lt 2 ]
then
	echo "usage: win(mac) tar"
	exit
fi

ftpdir=$1
tarname=$2

ftp -n<<!
open 192.168.1.121
user safeimager HZOPhqiPG11XRI
passive mode
cd $ftpdir
binary
put $tarname
bye
!

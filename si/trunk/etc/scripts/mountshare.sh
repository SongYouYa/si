#!/bin/bash


ethname=''
function get_ethname () {
	local output=`ip a`
	local name_pt='.*[0-9]+:\s+(enp[0-9]+s[0-9]+):.+'
	if [[ $output =~ $name_pt ]]
	then
		ethname=${BASH_REMATCH[1]}
	else
		echo 'match eth name failed!'
	fi
}
get_ethname
echo $ethname

setip_cmd='ip addr add 192.168.1.210/24 dev '$ethname
echo $setip_cmd
`$setip_cmd`
if [ $? -ne 0 ]
then
	echo 'set ip failed!'
	exit 1
fi

exit 0

win：
3.7.4:
把affconfig.h, afflib.mak, unistd.h 拷贝替换win32下的文件
把zlib-1.2.3解压到win32下
把openssl-1.2放到mak文件对应目录
打开vs命令行，cd到win32目录，执行make afflib.lib
修改编译参数，编译debug和release版本的aff库

3.7.17:
https://github.com/sshock/AFFLIBv3/tree/v3.7.17
affconfig.h不需要了
不知为何 COMPILER_MODE = /MDd 不起作用了

linux：
编译安装openssl-1.0.2o.tar.gz
主工程可能需要安装curl

mac:
openssl使用主工程使用的1.1版本

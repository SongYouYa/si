osx加载驱动
sudo kextload MacPmem.kext
sudo kextunload -b com.google.MacPmem

查看状态：kextutil MacPmem.kext


10:11
System Integrity Protection：
csrutil status
csrutil disable


10:10
禁用kext signing：
nvram boot-args=kext-dev-mode=1
reboot
enable:
sudo nvram -d boot-args


rekal命令：
--repository_path
--profile
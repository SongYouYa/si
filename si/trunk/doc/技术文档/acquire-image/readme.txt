osx��������
sudo kextload MacPmem.kext
sudo kextunload -b com.google.MacPmem

�鿴״̬��kextutil MacPmem.kext


10:11
System Integrity Protection��
csrutil status
csrutil disable


10:10
����kext signing��
nvram boot-args=kext-dev-mode=1
reboot
enable:
sudo nvram -d boot-args


rekal���
--repository_path
--profile
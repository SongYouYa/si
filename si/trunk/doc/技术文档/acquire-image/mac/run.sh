#!/bin/bash
sudo kextcache -invalidate /
sudo chown -R root:wheel ./MacPmem.kext
sudo chmod -R 755 ./MacPmem.kext
sudo kextload ./MacPmem.kext
dd if=/dev/pmem of=./osx.dd

#!/bin/bash
set -e

cd /mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-treasure/attachments
gdb -q -p "$1" -x capture.gdb > gdb_capture.txt 2>&1

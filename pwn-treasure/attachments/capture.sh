#!/bin/bash
set -e

cd /mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-treasure/attachments
rm -f debug_pid.txt gdb_capture.txt

python3 debug_pause.py >/tmp/dbg_pause.log 2>&1 &

for i in $(seq 1 30); do
    if [ -f debug_pid.txt ]; then
        break
    fi
    sleep 1
done

pid=$(cat debug_pid.txt)
gdb -q -p "$pid" -x capture.gdb > gdb_capture.txt 2>&1
kill "$pid" 2>/dev/null || true

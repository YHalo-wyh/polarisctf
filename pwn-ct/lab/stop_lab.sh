#!/usr/bin/env bash
set -euo pipefail

LAB="/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/lab"

for name in nginx-wrapper config-service device-api log-service; do
    pidfile="$LAB/run/$name.pid"
    if [[ ! -f "$pidfile" ]]; then
        continue
    fi
    pid="$(cat "$pidfile" 2>/dev/null || true)"
    if [[ -n "${pid:-}" ]] && kill -0 "$pid" 2>/dev/null; then
        kill "$pid" 2>/dev/null || true
        wait "$pid" 2>/dev/null || true
    fi
    rm -f "$pidfile"
done

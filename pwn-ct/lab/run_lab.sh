#!/usr/bin/env bash
set -euo pipefail

BASE="/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct"
LAB="$BASE/lab"
NGINX_BIN="/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/.local-nginx/root/usr/sbin/nginx"
CONF="$LAB/nginx.conf"

mkdir -p "$LAB"/{logs,run,html,static,app}
mkdir -p "$LAB/run"/{body_temp,proxy_temp,fastcgi_temp,uwsgi_temp,scgi_temp}

cat >"$LAB/html/index.html" <<'EOF'
<html><body><h1>iot-guardian local lab</h1></body></html>
EOF

cat >"$LAB/static/hello.txt" <<'EOF'
static content is reachable
EOF

cat >"$LAB/app/config.yaml" <<'EOF'
jwt_secret: PolarisCTF_local_alias_secret
issuer: iot-guardian
EOF

for name in config-service device-api log-service nginx-wrapper; do
    pidfile="$LAB/run/$name.pid"
    if [[ -f "$pidfile" ]]; then
        pid="$(cat "$pidfile" 2>/dev/null || true)"
        if [[ -n "${pid:-}" ]] && kill -0 "$pid" 2>/dev/null; then
            kill "$pid" 2>/dev/null || true
            wait "$pid" 2>/dev/null || true
        fi
        rm -f "$pidfile"
    fi
done

nohup python3 "$LAB/backend.py" 3001 "$LAB/app/config.yaml" >"$LAB/logs/device-api.log" 2>&1 &
echo $! >"$LAB/run/device-api.pid"

nohup python3 "$LAB/backend.py" 3002 "$LAB/app/config.yaml" >"$LAB/logs/config-service.log" 2>&1 &
echo $! >"$LAB/run/config-service.pid"

nohup python3 "$LAB/backend.py" 3003 "$LAB/app/config.yaml" >"$LAB/logs/log-service.log" 2>&1 &
echo $! >"$LAB/run/log-service.pid"

"$NGINX_BIN" -t -c "$CONF" >/dev/null
nohup "$NGINX_BIN" -c "$CONF" -g 'daemon off;' >"$LAB/logs/nginx.stdout.log" 2>&1 &
echo $! >"$LAB/run/nginx-wrapper.pid"

sleep 1
echo "lab ready at http://127.0.0.1:8080"

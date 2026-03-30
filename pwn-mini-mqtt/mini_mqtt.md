# PolarisCTF `minimqtt` 题解

## 题目类型

这题表面上给了一个 `pwn` 二进制、`libc.so.6` 和 `libpaho-mqtt3c.so.1`，看起来像常规 ELF 题，但真正的漏洞不是栈溢出、UAF 之类的内存破坏，而是一个很隐蔽的**命令注入 + 全局字符串残留**组合漏洞。

最后利用的核心点有两个：

1. 文件名里允许分号 `;`，而且拼接进了 `popen("cat /home/ctf/%s", ...)`
2. 全局命令缓冲区 `cmd` 用 `memcpy(cmd, buf, strlen(buf))` 更新，**没有拷贝结尾 `\\0`**

这两个点叠在一起，变成了一个非常漂亮的两阶段利用。

最终拿到的 flag 是：

```text
polarisctf{2a9690c5-321f-48a7-9114-ad38f074a310}
```

---

## 一、拿到程序先看什么

我建议这题按下面的顺序看：

1. `main`，地址 `0x1e8d`
2. `msgarrvd`，地址 `0x1d24`
3. `http`，地址 `0x1409`
4. `msgsend`，地址 `0x1c12`

原因很简单：

- `main` 决定这题到底是怎么交互的
- `msgarrvd` 决定收到 MQTT 消息后会走到哪里
- `http` 是真正的业务逻辑和漏洞点
- `msgsend` 决定回显怎么回来

如果一上来就盯着 `http` 看，会看见一堆 `sscanf / strstr / snprintf / popen`，但不一定知道消息从哪来、结果往哪回。先从 `main` 搭好全局视角会轻松很多。

---

## 二、基础侦察

先看保护：

```bash
checksec --file=./pwn
```

结果大概是：

- Full RELRO
- Canary
- NX
- PIE

第一眼会让人以为这是个标准内存破坏题，但接着看字符串就会发现不对劲：

```bash
strings -a pwn
```

能看到很多非常关键的内容：

```text
MQTTClient_publishMessage
MQTTClient_subscribe
MQTTClient_connect
httpclient
GET /home/ctf/%63[^ "
ContentLength: %d
cat /home/ctf/%s
index_html
fault2
fault
HTTP
```

这些字符串几乎已经把题目方向明牌了：

- 这是个 MQTT 客户端程序
- 它订阅了某个主题
- 收到消息以后会把内容按 HTTP 请求格式解析
- 最后还会执行 `cat /home/ctf/%s`

这时就该把思路从“栈/堆”切到“业务逻辑/命令注入”。

---

## 三、`main`：先确认交互模型

`main` 是阅读入口。

从反汇编里可以看出它做了这些事：

1. 连接到 MQTT broker，默认地址是 `tcp://localhost:9999`
2. client id 是 `httpclient`
3. 订阅主题 `HTTP`
4. 每隔 1 秒调用一次 `msgsend("200")`

也就是说，程序其实是一个**挂在 MQTT broker 上的“HTTP 模拟客户端”**。

它会不断往 `HTTP` 主题发：

```json
{"clientid":"httpclient", "message":"200"}
```

这个点非常重要，因为它给了我们一个天然探针：

- 看见 `200` 心跳，说明目标客户端在线
- 看不见心跳，说明 broker 可能还活着，但真正执行逻辑的 `httpclient` 没挂上来

这也是为什么我打远程时一定先订阅、先等心跳，再决定是否发 payload。

---

## 四、`msgarrvd`：MQTT 消息如何进入业务逻辑

`msgarrvd` 是 MQTT 的消息回调。

它大致逻辑是：

1. 如果消息长得像 JSON，并且 `clientid == "httpclient"`，就直接忽略
2. 否则把消息正文丢给 `http()`

这里的意义是：

- 程序自己通过 `msgsend()` 发出来的 JSON 心跳/结果消息不会再次进入解析逻辑
- 只有我们主动发到 `HTTP` 主题上的“原始文本消息”，才会被当成 HTTP 请求处理

所以利用方式不是打传统 socket HTTP，而是：

1. 连接 MQTT broker
2. 往主题 `HTTP` 发布一段看起来像 HTTP 请求的字符串

---

## 五、`msgsend`：结果从哪里回来

`msgsend` 负责把结果包装成 JSON，再发回 `HTTP` 主题。

格式大概是：

```json
{"clientid":"httpclient", "message":"<内容>"}
```

所以一旦 `http()` 里执行了命令、读到了某行输出，我们最终在 MQTT 侧看到的不是裸字符串，而是：

```json
{"clientid":"httpclient", "message":"polarisctf{...}"}
```

回显通道也就明确了：

- 输入通道：`HTTP` topic
- 输出通道：还是 `HTTP` topic

---

## 六、`http`：真正的漏洞点

### 1. 它在解析什么

`http()` 会把 MQTT 消息当作 HTTP 请求文本处理，并尝试从 GET 路径里取出“文件名”。

从字符串可以看到它支持的格式类似：

```text
GET /home/ctf/%63[^ "
GET %*[^/]/ctf/%63[^ "
GET %*s/ctf/"%63[^"]"
```

本质上就是：从 `/ctf/` 后面抠一个文件名出来。

### 2. 它如何构造命令

拿到文件名后，它会做一件致命的事：

```c
snprintf(local_cmd, 0x80, "cat /home/ctf/%s", filename);
memcpy(cmd, local_cmd, strlen(local_cmd));
```

然后在满足条件时执行：

```c
popen(cmd, "r");
```

这说明：

- 最终执行的是 shell 命令
- 文件名如果能混进 shell 元字符，就有命令注入的可能

### 3. 表面上的过滤

程序不是完全没过滤，它做了两层限制：

1. 逐字节把 `/` 和 `.` 替换成 `_`
2. 只允许真正读取 `index_html`

从逻辑上看，它本来想做的是：

- 只允许访问 `/home/ctf/index_html`
- 不允许路径穿越

但是它漏了两个关键问题。

---

## 七、漏洞 1：过滤了 `/` 和 `.`, 却没有过滤 `;`

程序只把 `/` 和 `.` 改成 `_`，但是像下面这些字符都还活着：

- `;`
- `$`
- `{`
- `}`
- `*`
- `|`

所以我们完全可以把文件名写成：

```text
index_html;cat${IFS}flag*
```

拼接后就会变成：

```bash
cat /home/ctf/index_html;cat${IFS}flag*
```

这里有两个额外小细节：

1. 不能出现空格，因为文件名的 `sscanf` 在空格处截断
2. `/` 会被改成 `_`，所以不能直接写 `/flag`

因此 payload 要稍微做一下 shell 级绕过：

- 用 `${IFS}` 代替空格
- 优先尝试相对路径 `flag*`
- 如果要造 `/`，可以用 `${PWD:0:1}` 或 `${PATH:0:1}`

这就是为什么最终 payload 写成：

```bash
cat${IFS}flag*||cat${IFS}${PWD:0:1}flag*
```

---

## 八、漏洞 2：`memcpy(strlen)` 没有拷贝结尾 `\0`

这是整题最精华的点。

程序把构造好的命令写到全局变量 `cmd` 时，用的是：

```c
memcpy(cmd, local_cmd, strlen(local_cmd));
```

注意：`strlen(local_cmd)` **不包含末尾的 `\0`**。

这意味着：

- 如果先写入一个长字符串
- 再用一个更短的字符串覆盖前缀

那么旧字符串尾巴会残留下来。

举个最关键的例子：

第一阶段先把 `cmd` 写成：

```bash
cat /home/ctf/index_html;cat${IFS}flag*
```

第二阶段再写入一个更短的正常命令：

```bash
cat /home/ctf/index_html
```

由于没有写入结尾 `\0`，全局 `cmd` 实际上会变成：

```bash
cat /home/ctf/index_html;cat${IFS}flag*
```

也就是说，第二次“看起来”把命令改回正常了，实际上只是把前半段改了，后面恶意后缀还在。

---

## 九、为什么必须两阶段利用

程序还有一道逻辑门：

- 只有文件名严格等于 `index_html`
- 且 `ContentLength <= 10`
- 且长度检查通过

它才会真的 `popen(cmd, "r")`

这就导致单次直接打：

```text
GET /home/ctf/index_html;cat${IFS}flag* HTTP/1.1
```

虽然能把恶意命令写进 `cmd`，但文件名已经不是 `index_html` 了，会被拒掉。

这时第二个漏洞就派上用场了：我们不要求第一阶段直接执行，只要求它**把恶意后缀种进全局 `cmd`**。

### 第一阶段：投毒

发送：

```http
GET /home/ctf/index_html;cat${IFS}flag*||cat${IFS}${PWD:0:1}flag* HTTP/1.1
Host: x
```

这一步的结果：

1. 程序从路径里取出整个恶意文件名
2. 生成长命令并写入全局 `cmd`
3. 因为文件名不等于 `index_html`，这次不会正常执行

但没关系，我们的目的已经达到了：**恶意后缀进了 `cmd`**。

### 第二阶段：触发

再发送：

```http
GET /home/ctf/index_html HTTP/1.1
Host: x
ContentLength: 10
```

这一步的结果：

1. 文件名是合法的 `index_html`
2. `ContentLength: 10` 正好满足检查
3. 新命令更短，只覆盖了前缀，没有写入 `\0`
4. 前一阶段的恶意后缀还留在 `cmd` 后面
5. `popen(cmd, "r")` 最终执行了拼好的恶意命令

这就是完整的利用链。

---

## 十、完整利用思路

### Step 1. 先确认远程实例是不是活的

连接 MQTT broker，订阅 `HTTP` 主题。

如果能持续看到：

```json
{"clientid":"httpclient", "message":"200"}
```

说明目标在线，可以打。

如果 broker 能连上，但看不到 `200`，通常说明：

- broker 还活着
- 但真正执行逻辑的 `httpclient` 没挂上来

这时候最好别浪费机会。

### Step 2. 发第一阶段 payload

```http
GET /home/ctf/index_html;cat${IFS}flag*||cat${IFS}${PWD:0:1}flag* HTTP/1.1
Host: x
```

作用：把恶意后缀写进全局 `cmd`。

### Step 3. 发第二阶段 payload

```http
GET /home/ctf/index_html HTTP/1.1
Host: x
ContentLength: 10
```

作用：通过合法性检查，并触发 `popen(cmd, "r")`。

### Step 4. 从 MQTT 回显里读 flag

最后会收到类似：

```json
{"clientid":"httpclient", "message":"polarisctf{2a9690c5-321f-48a7-9114-ad38f074a310}"}
```

---

## 十一、利用脚本思路

我最后的利用脚本做的事情其实不复杂：

1. 连接 broker
2. 订阅 `HTTP`
3. 等 `httpclient` 心跳
4. 发送两次 MQTT `PUBLISH`
5. 在同一主题上等回显

核心 payload 就这两个：

```python
stage1 = (
    "GET /home/ctf/index_html;"
    "cat${IFS}flag*||cat${IFS}${PWD:0:1}flag* "
    "HTTP/1.1\\r\\nHost: x\\r\\n"
)

stage2 = (
    "GET /home/ctf/index_html HTTP/1.1\\r\\n"
    "Host: x\\r\\n"
    "ContentLength: 10\\r\\n"
)
```

为什么这里优先读 `flag*`？

- 因为 `pwd` 已经确认工作目录是 `/home/ctf`
- 相对路径最短，也不怕 `/` 被过滤

为什么还要再补一个 `||cat${IFS}${PWD:0:1}flag*`？

- 这是兜底
- 万一 flag 不在当前目录，至少还能尝试 `/flag*`

---

## 十二、这题的本质

这题本质上不是传统 pwn，而是一个**状态型命令注入**：

### 漏洞根因

1. 把用户控制的文件名拼进 shell 命令
2. 过滤不完整，只处理了 `/` 和 `.`
3. 用 `memcpy(strlen(...))` 更新全局命令，遗漏 `\0`
4. 同一个全局 `cmd` 在多次请求之间复用

### 触发条件

1. 第一次请求负责“种入”恶意后缀
2. 第二次请求负责“合法触发”执行

这是一类非常值得记住的题型：**跨请求状态污染**。

单看每次请求都像是安全检查存在：

- 路径有限制
- 文件名有限制
- ContentLength 有限制

但由于全局状态更新不完整，最后还是能被两阶段绕过去。

---

## 十三、复盘时最值得记住的点

### 1. 先看 `main` 再看漏洞函数

`main -> callback -> worker -> send`

这个顺序能让你快速知道：

- 怎么进
- 怎么出
- 在线探针是什么

### 2. `strings` 非常关键

这题如果只看保护很容易走偏。

但只要看见：

```text
GET /home/ctf/%63[^ "
cat /home/ctf/%s
HTTP
httpclient
```

就该立刻切思路。

### 3. `memcpy(strlen(...))` 是经典坑

只要目标缓冲区是“复用的全局/静态缓冲区”，不拷贝 `\0` 就可能形成：

- 后缀残留
- 跨请求污染
- 逻辑绕过

这题就是一个非常标准、非常漂亮的例子。

---

## 十四、最终利用过程简表

```text
订阅 HTTP
    ↓
等到 {"clientid":"httpclient","message":"200"}
    ↓
发送 stage1：写入长命令到全局 cmd
    ↓
发送 stage2：用合法短命令覆盖前缀，但不覆盖恶意后缀
    ↓
程序 popen(cmd, "r")
    ↓
flag 通过 msgsend 发回 HTTP 主题
```

---

## 十五、最终 flag

```text
polarisctf{2a9690c5-321f-48a7-9114-ad38f074a310}
```
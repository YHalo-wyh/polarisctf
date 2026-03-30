# pwn-throne-hazard WP

## 题目概览

这题是一个典型的菜单堆题 + 竞态题。

核心思路不是去打栈，也不是去做传统 GOT 覆写，而是：

1. 先在菜单逻辑里找到真正有价值的几个功能点。
2. 再顺着 `forge` 和后台线程的联动，确认存在一个可控长度的竞态堆溢出。
3. 利用这个溢出把相邻的 `actuator` 结构体改造成“任意读 / 任意写 / 任意函数调用载体”。
4. 因为程序开了 seccomp，不能直接 `system("/bin/sh")` 或 `execve("/bin/sh", ...)`，所以最终走 ORW：`open -> readv -> writev` 读 flag。

远程最终 flag：

```text
polarisctf{325e1f34-85f0-48ae-9b97-4ba31bd21388}
```

---

## 保护与利用方向

程序特征：

- `No PIE`：程序本体地址固定，`.data/.bss`、自定义函数表地址都固定。
- `Full RELRO`：GOT 只读，不能走经典 GOT overwrite。
- `Canary`：不适合打栈溢出。
- `NX`：不能直接下 shellcode。
- `IBT/SHSTK`：说明启用了 CET 相关保护，传统随便乱跳 gadget 的利用链更麻烦。
- seccomp 存在。

seccomp 规则可以直接看出来，核心是：

- 禁止 `execve / execveat`
- 禁止 `mmap / mprotect / pkey_mprotect / mremap`
- 禁止 `clone / fork / vfork`
- 其余 syscall 基本允许

所以这题最自然的终点不是 shell，而是 ORW。

---

## 逆向时先看哪些函数

这题虽然被 strip 了，但菜单分发非常清楚。建议按下面顺序看。

### 1. 先看主菜单 `0x4012b0`

主函数在 `0x4012b0`，里面做了几件很重要的事：

- 初始化 IO
- `signal(SIGALRM, ...)` + `alarm(0x78)`
- `pthread_create` 启一个后台线程
- `prctl` 装 seccomp
- 打印菜单
- 用一个 jump table 分发 1~8 号菜单

菜单跳转表在 `0x402520`，对应关系如下：

| 菜单 | 地址 | 含义 |
| --- | --- | --- |
| 1 | `0x401710` | Calibrate self-optimizer target |
| 2 | `0x401640` | Forge memory capsule |
| 3 | `0x401758` | Build actuator |
| 4 | `0x4015d0` | Tune actuator seed |
| 5 | `0x4015a0` | Compose broadcast line |
| 6 | `0x401540` | Dispatch actuator |
| 7 | `0x4014b0` | Review arena |
| 8 | `0x401478` | Exit |

这一步的意义是先把题的骨架搭出来，别一上来就盯着某个 read。

### 2. 再看数字输入函数 `0x401b70`

`0x401b70` 是一个“读数字并做范围检查”的 helper。

它会：

- 打印提示字符串
- 读一行
- `strtoul`
- 检查解析是否成功
- 检查值是否在 `[min, max]` 范围内

因此菜单 1 的目标值范围是严格受控的：

```text
appeal target (0x20-0x78)
```

也就是最多只能设到 `0x78`。

### 3. 再看菜单 2：`forge`，地址 `0x401640`

这是全题最关键的函数。

先把逻辑简化一下：

1. 如果 `capsule` 还没分配，就 `calloc(1, 0x30)`。
2. 把一个全局“baseline”值设成 `0x20`。
3. 打印 `operator baseline sampled`。
4. 把一个全局状态位设成 `1`，通知后台线程开始工作。
5. 先读 **1 个字节** 到 `capsule[0]`。
6. 然后再按某个全局长度继续读剩余数据到 `capsule + 1`。
7. 最后把 baseline 重置回 `0x20`，状态位清零。

关键指令在这里：

```text
0x401686: mov [0x404140], 1          ; 通知后台线程开始
0x40168b: read_exact(capsule, 1)     ; 先读 1 字节
0x4016b4: mov r8d, [0x4040a4]
0x4016bd: add r8, 0xf
0x4016da: read_exact(capsule + 1, r8)
0x4016f4: mov [0x4040a4], 0x20       ; 恢复 baseline
0x4016ea: mov [0x404140], 0          ; 结束
```

这里要特别注意：

- 先读 1 字节
- 再按 `global_size + 0xf` 读剩余部分

如果 `global_size = 0x20`，那总长度就是：

```text
1 + (0x20 + 0xf) = 0x30
```

恰好填满 `calloc(1, 0x30)` 的 chunk。

### 4. 再看后台线程 `0x401df0`

这是题目的竞态来源。

线程会一直循环，只有在全局标志位 `0x404140 == 1` 时，才进入一次“优化周期”。

核心逻辑：

1. 读取当前 `appeal target`，地址 `0x4040a0`
2. 随机 `usleep`
3. 把 `0x4040a4` 改成这个 target
4. 再随机 `usleep`
5. 把 `0x4040a4` 改回 `0x20`
6. 把状态位 `0x404140` 清零

也就是说，`forge` 的第二段读取长度不是稳定的 `0x20 + 0xf`，而是可能在一个很短时间窗里变成：

```text
0x78 + 0xf = 0x87
```

于是总写入长度变成：

```text
1 + 0x87 = 0x88
```

这就是漏洞本体。

### 5. 然后看菜单 3：`build actuator`，地址 `0x401758`

它会 `malloc(0x48)` 创建一个 actuator，并初始化若干字段。

从初始化和后续使用看，`actuator` 结构里最有用的字段大概是：

```c
struct actuator {
    char tag[0x10];      // 前面是 "sentinel-9" 一类标识
    uint32_t lane;       // +0x10
    uint32_t active;     // +0x14
    uint64_t len;        // +0x18
    void *data;          // +0x20
    char inline_buf[0x20]; // +0x28
};
```

和利用强相关的是：

- `lane` 在 `+0x10`
- `len` 在 `+0x18`
- `data` 在 `+0x20`
- `+0x28` 后面有一段可直接当字符串用的 inline 空间

### 6. 再看菜单 6：`dispatch actuator`，地址 `0x401540`

这是利用真正的执行点。

关键逻辑：

```text
lane = *(uint32_t *)(act + 0x10)
len  = *(uint64_t *)(act + 0x18)
ptr  = *(uint64_t *)(act + 0x20)
call qword ptr [0x4040e0 + lane*8](ptr, len, lane)
```

所以它实际上是一个“通过 actuator 字段驱动的三参函数调用器”。

而 `0x4040e0` 这张表初始内容是：

| lane | 地址 | 实际效果 |
| --- | --- | --- |
| 0 | `0x401990` | `write(1, ptr, len)` |
| 1 | `0x401de0` | `read(0, ptr, len)` |
| 2 | `0x401a10` | FNV-1a digest 输出 |
| 3 | `0x4019d0` | `memset(ptr, 0, len)` 后打印 `scrubbed` |

这一步非常关键，因为这直接告诉我们：

- lane 0 天生就是任意读
- lane 1 天生就是任意写
- lane 3 只要函数表项可改，就能变成我们自己的三参调用器

### 7. 最后看菜单 7：`review arena`，地址 `0x4014b0`

这个功能用来打印当前 arena 状态。

它会打印：

- `supremacy floor`
- `appeal target`
- capsule 是否在线
- actuator 的 `lane` 和 `len`

这个功能的作用不是泄露内存，而是帮助我们确认竞态有没有命中：

- 如果溢出成功，`lane` 和 `len` 会变成我们伪造的值。
- 所以它可以直接作为 exploit 脚本里的命中判据。

---

## 全局变量和关键地址

利用里真正用到的全局地址如下：

| 地址 | 含义 |
| --- | --- |
| `0x4040a0` | appeal target |
| `0x4040a4` | forge 阶段动态长度基准 |
| `0x4040e0` | dispatch table 起点 |
| `0x4040f8` | dispatch table 的 lane 3 表项 |
| `0x404140` | 后台线程工作标志 |
| `0x4041e0` | actuator 指针 |
| `0x4041f0` | capsule 对外显示的容量 |
| `0x4041f8` | capsule 指针 |
| `0x404150` | 可用 `.bss` scratch 区，放 iovec |
| `0x404180` | 可用 `.bss` scratch 区，放路径/读入内容 |

`0x4040e0` 的表项在 `.data`，而不是 GOT。

这意味着：

- 虽然 Full RELRO 让 GOT 不能改
- 但我们仍然可以用任意写直接改 `dispatch table`

这也是这题真正可利用的原因。

---

## 漏洞点到底是什么

一句话概括：

**菜单 2 在同一次 forge 过程中，先读了 1 字节，再根据一个会被后台线程异步修改的全局长度继续读剩余数据，导致本应写满 0x30 chunk 的输入，在竞态命中时可以写到 0x88 字节，从而溢出到相邻的 actuator chunk。**

更细一点：

### 正常情况

- `capsule = calloc(1, 0x30)`
- baseline = `0x20`
- 第二段长度 = `0x20 + 0xf = 0x2f`
- 总长度 = `1 + 0x2f = 0x30`

完全不溢出。

### 竞态命中时

- 我们先通过菜单 1 把 `appeal target` 设成最大值 `0x78`
- forge 开始后，后台线程会在某个瞬间把 `0x4040a4` 改成 `0x78`
- forge 第二段读取长度变成 `0x78 + 0xf = 0x87`
- 总长度变成 `0x88`

而目标 chunk 只有 `0x30`，于是往后多写：

```text
0x88 - 0x30 = 0x58
```

足够覆盖：

- 下一个 chunk 的 metadata
- 紧接着分配的 actuator 结构体关键字段

---

## 为什么 `capsule` 后面就是 `actuator`

利用前我们做的准备是固定的：

1. 先菜单 2，确保 `capsule` 被分配
2. 再菜单 3，确保 `actuator` 被分配

此时堆布局大致是：

```text
[ capsule chunk | user size 0x30 ]
[ actuator chunk | user size 0x48 ]
```

chunk header 角度看：

- `capsule` 对应 chunk size 是 `0x40`
- `actuator` 对应 chunk size 是 `0x50`

所以溢出 payload 里要显式把下一个 chunk 的 size 修回 `0x51`：

- `0x50` 是 chunk size
- 最低位 `PREV_INUSE = 1`
- 所以写成 `0x51`

这就是 payload 里这两项的意义：

```text
offset 0x30 -> fake prev_size = 0
offset 0x38 -> fake size = 0x51
```

如果不把 chunk header 保住，glibc 后续很容易直接炸掉。

---

## 溢出后怎么把 actuator 变成利用原语

我们最终使用的覆盖布局如下：

```text
payload[0x00:0x08] = b"sentinel"
payload[0x30:0x38] = p64(0)
payload[0x38:0x40] = p64(0x51)
payload[0x50:0x54] = p32(lane)
payload[0x58:0x60] = p64(length)
payload[0x60:0x68] = p64(data_ptr)
payload[0x68:0x88] = inline bytes
```

原因是：

- `0x50` 正好落在 `actuator + 0x10`
- `0x58` 正好落在 `actuator + 0x18`
- `0x60` 正好落在 `actuator + 0x20`
- `0x68` 正好落在 `actuator + 0x28`

于是可以把 actuator 精确伪造成：

- `lane = 0` 时：`write(1, data_ptr, len)`，任意读
- `lane = 1` 时：`read(0, data_ptr, len)`，任意写
- `lane = 3` 时：调用 `dispatch_table[3]`

这就是本题完整利用链的核心。

---

## 为什么最终不用 shell，而用 ORW

这一步一定要讲清楚，因为这是很多人做这题最容易走歪的地方。

这题虽然能做到很强的原语，但直接 shell 并不好打，原因有三个：

1. seccomp 明确把 `execve`、`execveat` 禁了。
2. `mmap/mprotect` 也被禁了，不适合现铺 shellcode。
3. 开了 CET，做随便的 ROP 也不如走现成函数调用自然。

反过来，程序自己就给了我们一个三参调用器：

```text
call table[lane](rdi=ptr, rsi=len, rdx=lane)
```

只要把 `table[3]` 改成 libc 里的函数，就能非常自然地打 ORW。

---

## 利用链设计

最终利用链分成五步。

### 第一步：用 lane 0 泄露 `read@got`

先把 actuator 改成：

```text
lane = 0
len  = 8
ptr  = read@got
```

再 dispatch：

```text
write(1, read@got, 8)
```

就能拿到 `read` 的 libc 地址，进而算出 libc base。

因为程序 `No PIE`，`read@got` 地址固定；又因为题目附件给了 libc，所以基址计算是直接的：

```text
libc_base = leak(read@got) - offset(read)
```

### 第二步：用 lane 1 做任意写

把 actuator 改成：

```text
lane = 1
len  = n
ptr  = target_addr
```

dispatch 后实际就是：

```text
read(0, target_addr, n)
```

于是我们获得稳定任意写。

### 第三步：先在 `.bss` 准备 ORW 需要的数据

为了减少 race 次数，最终远程版没有再去 leak `capsule` 内联地址，而是直接往固定 `.bss` 写数据。

我们选了：

- `IOV_ADDR = 0x404150`
- `BUF_ADDR = 0x404180`

在 `IOV_ADDR` 写入：

```c
struct iovec iov[3];
iov[0].iov_base = BUF_ADDR;
iov[0].iov_len  = 0x80;
iov[1] = {0};
iov[2] = {0};
```

然后把 flag 路径字符串直接写到 `BUF_ADDR`。

这样后面：

- `open` 读路径时用 `BUF_ADDR`
- `readv/writev` 读写时也复用同一个 `iov`

### 第四步：覆盖 `dispatch_table[3] = open`

`lane 3` 原来指向的是 scrub 函数，地址在：

```text
TABLE3 = 0x4040f8
```

把它改成 libc 的 `open` 后，dispatch 的寄存器状态是：

```text
rdi = ptr
rsi = len
rdx = lane = 3
```

于是只要伪造：

```text
lane = 3
ptr  = BUF_ADDR
len  = 0
```

就相当于：

```c
open((char *)BUF_ADDR, 0, 3);
```

这里第三个参数 `mode=3` 没关系，因为 `flags=0` 是只读打开，`mode` 会被忽略。

### 第五步：依次改成 `readv` 和 `writev`

继续把 `dispatch_table[3]` 改成 `readv`。

此时 dispatch 时寄存器是：

```text
rdi = ptr
rsi = len
rdx = lane = 3
```

所以我们设置：

```text
ptr = 3
len = IOV_ADDR
lane = 3
```

得到：

```c
readv(3, (struct iovec *)IOV_ADDR, 3);
```

这里默认假设 `open` 成功后返回 fd=3。

这在此题里是成立的，因为：

- 标准输入是 0
- 标准输出是 1
- 标准错误是 2
- 程序没有再额外打开别的文件

最后把 `dispatch_table[3]` 改成 `writev`，再设置：

```text
ptr = 1
len = IOV_ADDR
lane = 3
```

就变成：

```c
writev(1, (struct iovec *)IOV_ADDR, 3);
```

直接把刚才读进 `BUF_ADDR` 的内容打印出来。

---

## 为什么这条链很顺

这题有一个很漂亮的点：`lane = 3` 恰好天生可以当 `iovcnt = 3`。

也就是说，dispatch 的第三个参数我们本来没法自由控制，但对于 `readv/writev` 来说：

```text
rdx = 3
```

正好就是一个合法的 `iovcnt`。

所以整个 ORW 链几乎是“卡进函数原型里”的：

- `open(path, 0, 3)`
- `readv(3, iov, 3)`
- `writev(1, iov, 3)`

这也是为什么最后选 `open/readv/writev`，而不是别的函数组合。

---

## 本地打通过程

本地阶段建议按下面顺序验证。

### 1. 先只验证竞态是否命中

方法很简单：

- 菜单 2 分配 capsule
- 菜单 3 分配 actuator
- 菜单 1 把 target 设为 `0x78`
- 菜单 2 在 `forge primer (1 byte)` 处卡住，等一小段时间后发 payload
- 菜单 7 看 `lane` 和 `len` 有没有被改成伪造值

如果 `Review arena` 打印出你想要的：

```text
actuator : ... lane=0 len=0x8
```

就说明竞态窗口撞中了。

### 2. 再做单独任意读

把 actuator 改成：

```text
lane = 0
len  = 8
ptr  = read@got
```

确认能稳定泄出 libc 地址。

### 3. 再做单独任意写

把 actuator 改成：

```text
lane = 1
len  = 8
ptr  = 某个可写地址
```

确认 dispatch 后，标准输入的数据真的写进去了。

### 4. 本地先读一个无害文件

本地最开始我用的是：

```text
/etc/hostname
```

这样更容易判断 ORW 链本身是否正常。

本地成功输出：

```text
WYH1412
```

说明：

- libc 基址没算错
- `TABLE3` 覆写有效
- `open/readv/writev` 三连没问题

---

## 远程为什么还要再优化一次

本地和远程最大的差别不是链子，而是 race 的稳定性。

最初版本为了写路径，额外 leak 了一次 `capsule` 指针，再把路径塞进 `actuator + 0x28` 的 inline buffer。

本地这样没问题，但远程多一次 leak 就多一次 race 命中需求，稳定性明显下降。

所以远程最终版做了一个优化：

- 不再 leak `capsule`
- 直接把路径写到固定 `.bss` 的 `BUF_ADDR`

这样整条链减少了一轮额外原语切换，远程更容易打中。

最终远程稳定版流程就是：

1. race 命中，泄 `read@got`
2. 算 libc base
3. 任意写把 `iov + path` 写进 `.bss`
4. 把 `TABLE3` 改成 `open`
5. dispatch 打开 `/flag`
6. 把 `TABLE3` 改成 `readv`
7. dispatch 读入 flag
8. 把 `TABLE3` 改成 `writev`
9. dispatch 打印 flag

---

## 关键利用脚本

最终脚本在：

- [solve.py](C:\Users\WYH\Desktop\polarisctf招新赛\pwn\pwn-throne-hazard\solve.py)

脚本里最关键的几个函数：

- `payload_for(...)`
  - 构造覆盖 `actuator` 的竞态 payload
- `arm_once(...)`
  - 在 `forge primer (1 byte)` 后按等待时间发包
- `arm_until(...)`
  - 多次尝试直到 review 显示命中
- `leak(...)`
  - 通过 lane 0 做任意读
- `write_bytes(...)`
  - 通过 lane 1 做任意写
- `trigger_lane3(...)`
  - 把 lane 3 当作任意三参调用入口

---

## 最终利用总结

这题最值得记住的不是某个具体偏移，而是分析顺序：

1. **先找菜单分发**，搞清楚每个功能在干什么。
2. **再找输入 helper**，看数值/字符串输入边界。
3. **盯住会和线程交互的功能点**，因为题面已经暗示 race。
4. **确认溢出覆盖到什么对象**，不要只停留在“好像能溢出”。
5. **找到被覆盖对象的调用点**，把它变成真正的原语。
6. **结合 seccomp 决定终点是 shell 还是 ORW**。

这题里，最核心的一句话就是：

> 竞态把一个本来只能写满 `0x30` 的 capsule，扩成了最多 `0x88` 字节写入；而它后面恰好挨着一个带函数分发表的 actuator，于是一次堆溢出直接变成了任意读、任意写和任意函数调用。

这条链打通以后，剩下就是一个非常标准的 libc ORW。


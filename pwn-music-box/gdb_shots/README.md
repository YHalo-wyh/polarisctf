# GDB 看点

- `01_gc_before_compact.svg`
  - 位置：`GarbageClean+0x67`
  - 含义：GC 已经打印 `GC: Be quiet.`，马上要开始 compact。
  - 重点看 `gc+8`、`gc+12`、`gc+16`。

- `02_gc_switch_base.svg`
  - 位置：`GarbageClean+0x157`
  - 含义：写回新的 `gc.base`。
  - 重点看 `gc.base` 从 `space0` 切到 `space1`。

- `03_getslotoffset_disas.svg`
  - 位置：`GetSlotOffset`
  - 含义：这里只是读取 `slot[idx].off` 并返回，没有加锁。
  - 这就是 stale offset 的起点。

- `04_shownote_disas.svg`
  - 位置：`ShowNote`
  - 含义：先加锁，再用传进来的 offset 和当前 `gc.base` 拼 `ptr`。
  - 如果头校验失败，退回成 `off=0,len=8`，最后固定从 `ptr+8` 读 8 字节。

- `selected/*.txt`
  - 对应每张图的原始文本版本，方便直接搜索和复制。

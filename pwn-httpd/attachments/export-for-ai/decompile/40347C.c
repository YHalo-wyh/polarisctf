/*
 * func-name: sub_40347C
 * func-address: 0x40347c
 * callers: 0x403011, 0x4035a0, 0x403814
 * callees: 0x401440, 0x401460, 0x4014f0, 0x40326a
 */

void *__fastcall sub_40347C(__int64 a1)
{
  size_t v1; // rax
  void *ptr; // [rsp+18h] [rbp-8h]

  ptr = malloc(0x28141uLL);
  v1 = malloc_usable_size(ptr);
  memset(ptr, 0, v1);
  sub_40326A(*(char **)(a1 + 10856), (__int64)ptr);
  return ptr;
}

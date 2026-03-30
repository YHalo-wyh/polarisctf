/*
 * func-name: sub_402BDB
 * func-address: 0x402bdb
 * callers: 0x40213b, 0x40273e
 * callees: 0x401310, 0x401390, 0x4013c0, 0x401440, 0x401460, 0x4014f0, 0x401550, 0x402c9e, 0x402f5d
 */

int __fastcall sub_402BDB(unsigned int a1, const char *a2, unsigned int a3)
{
  size_t v3; // rax
  FILE *stream; // [rsp+18h] [rbp-18h]
  __int64 n; // [rsp+20h] [rbp-10h]
  void *ptr; // [rsp+28h] [rbp-8h]

  stream = fopen(a2, "r");
  n = sub_402F5D(stream);
  ptr = malloc(n + 1);
  v3 = malloc_usable_size(ptr);
  memset(ptr, 0, v3);
  fread(ptr, 1uLL, n, stream);
  sub_402C9E(a1, a3, "text/html", ptr);
  free(ptr);
  return fclose(stream);
}

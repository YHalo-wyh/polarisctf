/*
 * func-name: .memset
 * func-address: 0x401440
 * callers: 0x401a24, 0x401c58, 0x40213b, 0x402bdb, 0x4031ee, 0x40326a, 0x40347c
 * callees: none
 */

// attributes: thunk
void *memset(void *s, int c, size_t n)
{
  return memset(s, c, n);
}

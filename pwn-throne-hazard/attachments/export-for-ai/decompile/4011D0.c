/*
 * func-name: .memset
 * func-address: 0x4011d0
 * callers: 0x4019d0
 * callees: none
 */

// attributes: thunk
void *memset(void *s, int c, size_t n)
{
  return memset(s, c, n);
}

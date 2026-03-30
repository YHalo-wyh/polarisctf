/*
 * func-name: .memset
 * func-address: 0x2430
 * callers: 0x3570, 0x4d60
 * callees: none
 */

// attributes: thunk
void *memset(void *s, int c, size_t n)
{
  return memset(s, c, n);
}

/*
 * func-name: .memset
 * func-address: 0x11f0
 * callers: 0x15c7
 * callees: none
 */

// attributes: thunk
void *memset(void *s, int c, size_t n)
{
  return memset(s, c, n);
}

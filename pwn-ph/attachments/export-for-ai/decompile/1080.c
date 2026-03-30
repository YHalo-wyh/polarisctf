/*
 * func-name: .memcpy
 * func-address: 0x1080
 * callers: 0x1390
 * callees: none
 */

// attributes: thunk
void *memcpy(void *dest, const void *src, size_t n)
{
  return memcpy(dest, src, n);
}

/*
 * func-name: .memcpy
 * func-address: 0x2480
 * callers: 0x2d30, 0x6d00, 0x7e60
 * callees: none
 */

// attributes: thunk
void *memcpy(void *dest, const void *src, size_t n)
{
  return memcpy(dest, src, n);
}

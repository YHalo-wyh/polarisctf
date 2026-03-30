/*
 * func-name: .memcpy
 * func-address: 0x4014d0
 * callers: 0x401c58, 0x40326a, 0x4035a0, 0x403814
 * callees: none
 */

// attributes: thunk
void *memcpy(void *dest, const void *src, size_t n)
{
  return memcpy(dest, src, n);
}

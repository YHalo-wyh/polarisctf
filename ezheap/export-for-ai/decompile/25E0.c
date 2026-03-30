/*
 * func-name: .memmove
 * func-address: 0x25e0
 * callers: 0x6d00, 0x7e60
 * callees: none
 */

// attributes: thunk
void *memmove(void *dest, const void *src, size_t n)
{
  return memmove(dest, src, n);
}

/*
 * func-name: .malloc
 * func-address: 0x2550
 * callers: 0x36a0, 0x4d60
 * callees: none
 */

// attributes: thunk
void *malloc(size_t size)
{
  return malloc(size);
}

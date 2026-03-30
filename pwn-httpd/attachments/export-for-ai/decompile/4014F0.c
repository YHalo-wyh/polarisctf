/*
 * func-name: .malloc
 * func-address: 0x4014f0
 * callers: 0x401a24, 0x401c58, 0x402bdb, 0x40326a, 0x40347c
 * callees: none
 */

// attributes: thunk
void *malloc(size_t size)
{
  return malloc(size);
}

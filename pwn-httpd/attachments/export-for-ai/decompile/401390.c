/*
 * func-name: .fread
 * func-address: 0x401390
 * callers: 0x402bdb
 * callees: none
 */

// attributes: thunk
size_t fread(void *ptr, size_t size, size_t n, FILE *stream)
{
  return fread(ptr, size, n, stream);
}

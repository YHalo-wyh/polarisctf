/*
 * func-name: .calloc
 * func-address: 0x401200
 * callers: 0x4012b0
 * callees: none
 */

// attributes: thunk
void *calloc(size_t nmemb, size_t size)
{
  return calloc(nmemb, size);
}

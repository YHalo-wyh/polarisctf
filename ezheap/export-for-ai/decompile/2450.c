/*
 * func-name: .calloc
 * func-address: 0x2450
 * callers: 0x4230
 * callees: none
 */

// attributes: thunk
void *calloc(size_t nmemb, size_t size)
{
  return calloc(nmemb, size);
}

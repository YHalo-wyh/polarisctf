/*
 * func-name: .strncpy
 * func-address: 0x401340
 * callers: 0x401c58, 0x4031ee
 * callees: none
 */

// attributes: thunk
char *strncpy(char *dest, const char *src, size_t n)
{
  return strncpy(dest, src, n);
}

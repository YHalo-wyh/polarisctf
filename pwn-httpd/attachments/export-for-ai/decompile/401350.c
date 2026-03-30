/*
 * func-name: .strncmp
 * func-address: 0x401350
 * callers: 0x40273e, 0x40326a
 * callees: none
 */

// attributes: thunk
int strncmp(const char *s1, const char *s2, size_t n)
{
  return strncmp(s1, s2, n);
}

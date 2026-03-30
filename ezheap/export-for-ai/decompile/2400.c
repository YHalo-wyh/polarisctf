/*
 * func-name: .strlen
 * func-address: 0x2400
 * callers: 0x30a0, 0x4870, 0x5d60, 0x8a60
 * callees: none
 */

// attributes: thunk
size_t strlen(const char *s)
{
  return strlen(s);
}

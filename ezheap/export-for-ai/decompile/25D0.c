/*
 * func-name: .snprintf
 * func-address: 0x25d0
 * callers: 0x4d60, 0x6e90
 * callees: none
 */

// attributes: thunk
int snprintf(char *s, size_t maxlen, const char *format, ...)
{
  return snprintf(s, maxlen, format);
}

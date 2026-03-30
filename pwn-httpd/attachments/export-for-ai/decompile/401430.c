/*
 * func-name: .snprintf
 * func-address: 0x401430
 * callers: 0x40213b, 0x403105
 * callees: none
 */

// attributes: thunk
int snprintf(char *s, size_t maxlen, const char *format, ...)
{
  return snprintf(s, maxlen, format);
}

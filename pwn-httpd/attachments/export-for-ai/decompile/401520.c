/*
 * func-name: .fseek
 * func-address: 0x401520
 * callers: 0x402f5d
 * callees: none
 */

// attributes: thunk
int fseek(FILE *stream, __int64 off, int whence)
{
  return fseek(stream, off, whence);
}

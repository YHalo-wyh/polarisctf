/*
 * func-name: .setvbuf
 * func-address: 0x401260
 * callers: 0x4012b0
 * callees: none
 */

// attributes: thunk
int setvbuf(FILE *stream, char *buf, int modes, size_t n)
{
  return setvbuf(stream, buf, modes, n);
}

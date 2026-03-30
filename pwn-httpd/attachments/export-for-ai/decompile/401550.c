/*
 * func-name: .fopen
 * func-address: 0x401550
 * callers: 0x402bdb
 * callees: none
 */

// attributes: thunk
FILE *fopen(const char *filename, const char *modes)
{
  return fopen(filename, modes);
}

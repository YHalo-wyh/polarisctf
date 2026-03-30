/*
 * func-name: .open
 * func-address: 0x1250
 * callers: 0x1886
 * callees: none
 */

// attributes: thunk
int open(const char *file, int oflag, ...)
{
  return open(file, oflag);
}

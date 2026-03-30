/*
 * func-name: .write
 * func-address: 0x4011b0
 * callers: 0x4019d0
 * callees: none
 */

// attributes: thunk
ssize_t write(int fd, const void *buf, size_t n)
{
  return write(fd, buf, n);
}

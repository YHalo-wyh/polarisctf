/*
 * func-name: .write
 * func-address: 0x11a0
 * callers: 0x1d03
 * callees: none
 */

// attributes: thunk
ssize_t write(int fd, const void *buf, size_t n)
{
  return write(fd, buf, n);
}

/*
 * func-name: .write
 * func-address: 0x4013b0
 * callers: 0x40196e, 0x402c9e, 0x403105
 * callees: none
 */

// attributes: thunk
ssize_t write(int fd, const void *buf, size_t n)
{
  return write(fd, buf, n);
}

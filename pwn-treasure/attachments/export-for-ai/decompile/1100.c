/*
 * func-name: .read
 * func-address: 0x1100
 * callers: 0x1214, 0x12b3
 * callees: none
 */

// attributes: thunk
ssize_t read(int fd, void *buf, size_t nbytes)
{
  return read(fd, buf, nbytes);
}

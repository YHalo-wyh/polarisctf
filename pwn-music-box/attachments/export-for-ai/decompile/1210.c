/*
 * func-name: .read
 * func-address: 0x1210
 * callers: 0x1886, 0x19ff, 0x1aac, 0x1c3c
 * callees: none
 */

// attributes: thunk
ssize_t read(int fd, void *buf, size_t nbytes)
{
  return read(fd, buf, nbytes);
}

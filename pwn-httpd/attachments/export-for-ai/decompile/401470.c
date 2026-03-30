/*
 * func-name: .read
 * func-address: 0x401470
 * callers: 0x401c58
 * callees: none
 */

// attributes: thunk
ssize_t read(int fd, void *buf, size_t nbytes)
{
  return read(fd, buf, nbytes);
}

/*
 * func-name: .read
 * func-address: 0x4011f0
 * callers: 0x401aa0, 0x401d90
 * callees: none
 */

// attributes: thunk
ssize_t read(int fd, void *buf, size_t nbytes)
{
  return read(fd, buf, nbytes);
}

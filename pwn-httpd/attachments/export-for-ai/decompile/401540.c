/*
 * func-name: .bind
 * func-address: 0x401540
 * callers: 0x4016e3
 * callees: none
 */

// attributes: thunk
int bind(int fd, const struct sockaddr *addr, socklen_t len)
{
  return bind(fd, addr, len);
}

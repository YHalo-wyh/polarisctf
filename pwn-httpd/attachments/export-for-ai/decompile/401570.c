/*
 * func-name: .accept
 * func-address: 0x401570
 * callers: 0x4016e3
 * callees: none
 */

// attributes: thunk
int accept(int fd, struct sockaddr *addr, socklen_t *addr_len)
{
  return accept(fd, addr, addr_len);
}

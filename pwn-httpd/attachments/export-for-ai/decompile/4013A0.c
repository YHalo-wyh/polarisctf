/*
 * func-name: .setsockopt
 * func-address: 0x4013a0
 * callers: 0x4016e3
 * callees: none
 */

// attributes: thunk
int setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
  return setsockopt(fd, level, optname, optval, optlen);
}

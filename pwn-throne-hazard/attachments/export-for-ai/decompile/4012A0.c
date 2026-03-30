/*
 * func-name: .usleep
 * func-address: 0x4012a0
 * callers: 0x401df0
 * callees: none
 */

// attributes: thunk
int usleep(__useconds_t useconds)
{
  return usleep(useconds);
}

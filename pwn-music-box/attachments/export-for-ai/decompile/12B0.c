/*
 * func-name: .usleep
 * func-address: 0x12b0
 * callers: 0x13a9, 0x1775, 0x1dd7
 * callees: none
 */

// attributes: thunk
int usleep(__useconds_t useconds)
{
  return usleep(useconds);
}

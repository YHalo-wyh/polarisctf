/*
 * func-name: .prctl
 * func-address: 0x401220
 * callers: 0x4012b0
 * callees: none
 */

// attributes: thunk
int prctl(int option, ...)
{
  return prctl(option);
}

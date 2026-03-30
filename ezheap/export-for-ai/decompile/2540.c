/*
 * func-name: .prctl
 * func-address: 0x2540
 * callers: 0x8a60
 * callees: none
 */

// attributes: thunk
int prctl(int option, ...)
{
  return prctl(option);
}

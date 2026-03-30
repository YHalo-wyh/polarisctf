/*
 * func-name: .signal
 * func-address: 0x4014a0
 * callers: 0x4016e3
 * callees: none
 */

// attributes: thunk
__sighandler_t signal(int sig, __sighandler_t handler)
{
  return signal(sig, handler);
}

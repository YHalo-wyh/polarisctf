/*
 * func-name: .signal
 * func-address: 0x401210
 * callers: 0x4012b0
 * callees: none
 */

// attributes: thunk
__sighandler_t signal(int sig, __sighandler_t handler)
{
  return signal(sig, handler);
}

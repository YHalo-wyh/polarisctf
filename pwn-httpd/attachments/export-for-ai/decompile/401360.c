/*
 * func-name: ._exit
 * func-address: 0x401360
 * callers: 0x40196e
 * callees: none
 */

// attributes: thunk
void __noreturn _exit(int status)
{
  exit(status);
}

/*
 * func-name: ._exit
 * func-address: 0x401180
 * callers: 0x4019b0, 0x4019d0, 0x401a80
 * callees: none
 */

// attributes: thunk
void __noreturn _exit(int status)
{
  exit(status);
}

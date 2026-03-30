/*
 * func-name: .__stack_chk_fail
 * func-address: 0x11c0
 * callers: 0x19ff
 * callees: none
 */

// attributes: thunk
void __noreturn __stack_chk_fail()
{
  _stack_chk_fail();
}

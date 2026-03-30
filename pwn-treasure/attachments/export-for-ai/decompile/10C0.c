/*
 * func-name: .__stack_chk_fail
 * func-address: 0x10c0
 * callers: 0x12b3
 * callees: none
 */

// attributes: thunk
void __noreturn __stack_chk_fail()
{
  _stack_chk_fail();
}

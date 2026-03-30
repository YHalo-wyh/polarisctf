/*
 * func-name: .__stack_chk_fail
 * func-address: 0x4011c0
 * callers: 0x4012b0, 0x401aa0, 0x401b70, 0x401c50
 * callees: none
 */

// attributes: thunk
void __noreturn __stack_chk_fail()
{
  _stack_chk_fail();
}

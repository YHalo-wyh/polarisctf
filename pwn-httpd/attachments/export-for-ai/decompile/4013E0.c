/*
 * func-name: .__stack_chk_fail
 * func-address: 0x4013e0
 * callers: 0x401c58, 0x40213b, 0x40273e, 0x402b86, 0x402c9e, 0x402e52, 0x403011, 0x403105, 0x4035a0, 0x403814
 * callees: none
 */

// attributes: thunk
void __noreturn __stack_chk_fail()
{
  _stack_chk_fail();
}

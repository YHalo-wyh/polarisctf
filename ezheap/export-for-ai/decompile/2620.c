/*
 * func-name: ._Unwind_Resume
 * func-address: 0x2620
 * callers: 0x27a0, 0x3210, 0x3910, 0x3df0, 0x4230, 0x4870, 0x4d60, 0x55b0, 0x5d60, 0x6750, 0x6e90, 0x7fb0, 0x8a60
 * callees: none
 */

// attributes: thunk
void __noreturn _Unwind_Resume(struct _Unwind_Exception *a1)
{
  Unwind_Resume(a1);
}

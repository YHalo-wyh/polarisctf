/*
 * func-name: sub_11C0
 * func-address: 0x11c0
 * callers: none
 * callees: 0x10a0, 0x1150
 */

FILE **sub_11C0()
{
  FILE **result; // rax

  if ( !byte_4088 )
  {
    if ( &__cxa_finalize )
      _cxa_finalize(off_4058);
    result = sub_1150();
    byte_4088 = 1;
  }
  return result;
}

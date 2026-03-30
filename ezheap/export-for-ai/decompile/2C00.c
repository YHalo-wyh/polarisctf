/*
 * func-name: sub_2C00
 * func-address: 0x2c00
 * callers: none
 * callees: 0x2350, 0x2b90
 */

void *sub_2C00()
{
  void *result; // rax

  if ( !byte_D390 )
  {
    if ( &__cxa_finalize )
      _cxa_finalize(off_D008);
    result = sub_2B90();
    byte_D390 = 1;
  }
  return result;
}

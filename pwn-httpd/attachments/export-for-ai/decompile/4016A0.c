/*
 * func-name: sub_4016A0
 * func-address: 0x4016a0
 * callers: none
 * callees: 0x401630
 */

char *sub_4016A0()
{
  char *result; // rax

  if ( !byte_406180 )
  {
    result = sub_401630();
    byte_406180 = 1;
  }
  return result;
}

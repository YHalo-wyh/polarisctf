/*
 * func-name: sub_401950
 * func-address: 0x401950
 * callers: none
 * callees: 0x4018e0
 */

FILE **sub_401950()
{
  FILE **result; // rax

  if ( !byte_404128 )
  {
    result = sub_4018E0();
    byte_404128 = 1;
  }
  return result;
}

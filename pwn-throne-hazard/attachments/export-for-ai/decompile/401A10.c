/*
 * func-name: sub_401A10
 * func-address: 0x401a10
 * callers: none
 * callees: 0x401280
 */

__int64 __fastcall sub_401A10(unsigned __int8 *a1, __int64 a2)
{
  __int64 v2; // rcx
  unsigned __int8 *v4; // rsi
  __int64 v5; // rax

  v2 = 0xCBF29CE484222325LL;
  if ( a2 )
  {
    v4 = &a1[a2];
    do
    {
      v5 = *a1++;
      v2 = 0x100000001B3LL * (v5 ^ v2);
    }
    while ( v4 != a1 );
  }
  __dprintf_chk();
  return a2;
}

/*
 * func-name: sub_401ACD
 * func-address: 0x401acd
 * callers: 0x401b20
 * callees: none
 */

__int64 __fastcall sub_401ACD(char a1)
{
  if ( a1 > 47 && a1 <= 57 )
    return (unsigned int)(a1 - 48);
  if ( a1 > 64 && a1 <= 70 )
    return (unsigned int)(a1 - 55);
  if ( a1 <= 96 || a1 > 102 )
    return 0xFFFFFFFFLL;
  return (unsigned int)(a1 - 87);
}

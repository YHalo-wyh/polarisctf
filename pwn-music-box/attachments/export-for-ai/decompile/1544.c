/*
 * func-name: GetIndex
 * func-address: 0x1544
 * callers: 0x1aac
 * callees: none
 */

__int64 __fastcall GetIndex(__int64 a1)
{
  unsigned int i; // [rsp+14h] [rbp-4h]

  for ( i = 0; dword_4304 > i; ++i )
  {
    if ( dword_4308[2 * i + 516] != -1 && a1 == qword_4310 + dword_4308[2 * i + 516] )
      return i;
  }
  return 0xFFFFFFFFLL;
}

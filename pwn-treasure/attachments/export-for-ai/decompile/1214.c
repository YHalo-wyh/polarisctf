/*
 * func-name: sub_1214
 * func-address: 0x1214
 * callers: 0x12b3
 * callees: 0x1100
 */

unsigned __int64 __fastcall sub_1214(__int64 a1, __int64 a2, int a3)
{
  unsigned __int64 result; // rax
  unsigned int i; // [rsp+1Ch] [rbp-4h]

  for ( i = 0; ; ++i )
  {
    result = i;
    if ( (int)i >= a3 )
      break;
    read(0, (void *)((int)i + a2), 1uLL);
    if ( *(_BYTE *)((int)i + a2) == 10 || *(_BYTE *)((int)i + a2) == 32 )
    {
      result = (int)i + a2;
      *(_BYTE *)result = 0;
      return result;
    }
    if ( i == a3 )
      *(_BYTE *)(a3 + a2) = 0;
  }
  return result;
}

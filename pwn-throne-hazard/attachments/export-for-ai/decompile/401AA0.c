/*
 * func-name: sub_401AA0
 * func-address: 0x401aa0
 * callers: 0x401b70, 0x401c50
 * callees: 0x4011c0, 0x4011f0, 0x401a80
 */

__int64 __fastcall sub_401AA0(__int64 a1, __int64 a2)
{
  __int64 v2; // rbx
  __int64 v3; // r14
  char buf; // [rsp+7h] [rbp-31h] BYREF
  unsigned __int64 v6; // [rsp+8h] [rbp-30h]

  v2 = 1LL;
  v6 = __readfsqword(0x28u);
  buf = 0;
  while ( 1 )
  {
    v3 = v2 - 1;
    if ( read(0, &buf, 1uLL) <= 0 )
      sub_401A80("operator link lost");
    if ( buf == 10 )
      break;
    *(_BYTE *)(a1 + v2 - 1) = buf;
    if ( v2 + 1 == a2 )
    {
      while ( read(0, &buf, 1uLL) > 0 && buf != 10 )
        ;
      v3 = v2;
      break;
    }
    ++v2;
  }
  *(_BYTE *)(a1 + v3) = 0;
  return v3;
}

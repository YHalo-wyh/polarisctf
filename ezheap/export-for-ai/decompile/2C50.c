/*
 * func-name: session_postproc_default
 * func-address: 0x2c50
 * callers: 0x3910, 0x4d60, 0x5d60
 * callees: none
 */

void __fastcall session_postproc_default(__int64 a1)
{
  _BYTE *v1; // rax
  unsigned int i; // ecx
  _BYTE *v3; // rsi
  unsigned int v4; // edx
  char v5; // al

  if ( a1 )
  {
    v1 = *(_BYTE **)(a1 + 8);
    if ( v1 )
    {
      if ( *(_DWORD *)(a1 + 4) )
      {
        *v1 ^= 0x11u;
        for ( i = 1; i < *(_DWORD *)(a1 + 4); *v3 ^= v5 - v4 + 17 )
        {
          v3 = (_BYTE *)(i + *(_QWORD *)(a1 + 8));
          v4 = 7 * (i / 7);
          v5 = i++;
        }
      }
    }
  }
}

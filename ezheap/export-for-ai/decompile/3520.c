/*
 * func-name: session_postproc_shift_right
 * func-address: 0x3520
 * callers: 0x3910
 * callees: none
 */

void __fastcall session_postproc_shift_right(__int64 a1)
{
  _BYTE *v1; // rax
  unsigned int i; // edx
  _BYTE *v3; // rax

  if ( a1 )
  {
    v1 = *(_BYTE **)(a1 + 8);
    if ( v1 )
    {
      if ( *(_DWORD *)(a1 + 4) )
      {
        *v1 >>= 1;
        for ( i = 1; i < *(_DWORD *)(a1 + 4); *v3 >>= 1 )
          v3 = (_BYTE *)(*(_QWORD *)(a1 + 8) + i++);
      }
    }
  }
}

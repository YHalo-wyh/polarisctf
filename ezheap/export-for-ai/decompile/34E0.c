/*
 * func-name: session_postproc_clamp_negative
 * func-address: 0x34e0
 * callers: 0x3910
 * callees: none
 */

void __fastcall session_postproc_clamp_negative(__int64 a1)
{
  __int64 v1; // rax
  __int64 v2; // rdx
  char *v3; // rax

  if ( a1 )
  {
    v1 = *(_QWORD *)(a1 + 8);
    if ( v1 )
    {
      if ( *(_DWORD *)(a1 + 4) )
      {
        v2 = 0LL;
        while ( 1 )
        {
          v3 = (char *)(v2 + v1);
          if ( *v3 < 0 )
            *v3 = 0;
          if ( (unsigned int)++v2 >= *(_DWORD *)(a1 + 4) )
            break;
          v1 = *(_QWORD *)(a1 + 8);
        }
      }
    }
  }
}

/*
 * func-name: sub_4034DA
 * func-address: 0x4034da
 * callers: 0x403011, 0x4035a0, 0x403814
 * callees: 0x401490
 */

__int64 __fastcall sub_4034DA(const char *a1, __int64 a2, _QWORD *a3)
{
  unsigned int v5; // [rsp+28h] [rbp-8h]
  int i; // [rsp+2Ch] [rbp-4h]

  v5 = 0;
  for ( i = 0; i <= 19 && 8208LL * i + a2; ++i )
  {
    if ( !strcmp(a1, (const char *)(8208LL * i + a2)) )
    {
      *a3 = a2 + 8208LL * i;
      return 1;
    }
  }
  return v5;
}

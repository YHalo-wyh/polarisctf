/*
 * func-name: runtime_ctx_cleanup
 * func-address: 0x3570
 * callers: 0x27a0
 * callees: 0x2430, 0x24c0, 0x2510
 */

void __fastcall runtime_ctx_cleanup(_QWORD *a1)
{
  void *v2; // rdi
  void ***v3; // rbx
  void **v4; // rdi
  void **v5; // rbx
  void **v6; // r13
  __int64 v7; // rax
  void **v8; // rbx
  void *v9; // rdi

  v2 = (void *)a1[3];
  if ( v2 )
  {
    free(v2);
    a1[3] = 0LL;
  }
  v3 = (void ***)(a1 + 13);
  if ( a1 + 13 != a1 + 29 )
  {
    do
    {
      v4 = *v3;
      if ( *v3 )
      {
        if ( v4[1] )
        {
          free(v4[1]);
          v4 = *v3;
          (*v3)[1] = 0LL;
        }
        operator delete(v4, 0x50uLL);
        *v3 = 0LL;
      }
      ++v3;
    }
    while ( v3 != a1 + 29 );
  }
  memset(a1 + 45, 0, 0x10uLL);
  v5 = (void **)a1[47];
  v6 = (void **)a1[48];
  if ( v6 != v5 )
  {
    do
    {
      if ( *v5 )
      {
        operator delete(*v5, 0x50uLL);
        *v5 = 0LL;
      }
      ++v5;
    }
    while ( v6 != v5 );
    v7 = a1[47];
    if ( a1[48] != v7 )
      a1[48] = v7;
  }
  v8 = (void **)(a1 + 5);
  do
  {
    if ( *v8 )
    {
      free(*v8);
      *v8 = 0LL;
    }
    ++v8;
  }
  while ( v8 != a1 + 13 );
  v9 = (void *)a1[4];
  if ( v9 )
  {
    free(v9);
    a1[4] = 0LL;
  }
}

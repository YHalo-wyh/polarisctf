/*
 * func-name: sub_401D90
 * func-address: 0x401d90
 * callers: 0x4012b0, 0x401de0
 * callees: 0x4011f0, 0x401a80
 */

unsigned __int64 __fastcall sub_401D90(__int64 a1, unsigned __int64 a2)
{
  unsigned __int64 i; // r12
  ssize_t v3; // rax

  if ( !a2 )
    return 0LL;
  for ( i = 0LL; i < a2; i += v3 )
  {
    v3 = read(0, (void *)(a1 + i), a2 - i);
    if ( v3 <= 0 )
      sub_401A80("channel dropped");
  }
  return i;
}

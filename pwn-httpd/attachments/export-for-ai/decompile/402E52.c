/*
 * func-name: sub_402E52
 * func-address: 0x402e52
 * callers: 0x4019c2
 * callees: 0x4013e0, 0x401480, 0x4014e0, 0x4015d0
 */

unsigned __int64 __fastcall sub_402E52(__int64 a1, int a2)
{
  unsigned int v2; // eax
  int i; // [rsp+14h] [rbp-5Ch]
  char v5[72]; // [rsp+20h] [rbp-50h] BYREF
  unsigned __int64 v6; // [rsp+68h] [rbp-8h]

  v6 = __readfsqword(0x28u);
  v2 = time(0LL);
  srand(v2);
  strcpy(v5, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
  for ( i = 0; i < a2; ++i )
    *(_BYTE *)(a1 + i) = v5[rand() % 62];
  *(_BYTE *)(a2 + a1) = 0;
  return v6 - __readfsqword(0x28u);
}

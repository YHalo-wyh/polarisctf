/*
 * func-name: sub_402F5D
 * func-address: 0x402f5d
 * callers: 0x402bdb
 * callees: 0x4014c0, 0x401520
 */

__int64 __fastcall sub_402F5D(FILE *a1)
{
  __int64 v2; // [rsp+18h] [rbp-8h]

  fseek(a1, 0LL, 2);
  v2 = ftell(a1);
  fseek(a1, 0LL, 0);
  return v2;
}

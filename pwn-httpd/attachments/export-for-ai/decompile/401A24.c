/*
 * func-name: sub_401A24
 * func-address: 0x401a24
 * callers: 0x4016e3
 * callees: 0x401310, 0x401440, 0x401490, 0x4014f0, 0x401c58, 0x40213b, 0x40273e
 */

void __fastcall sub_401A24(unsigned int a1)
{
  void *s; // [rsp+18h] [rbp-8h]

  s = malloc(0x2A78uLL);
  memset(s, 0, 0x2A78uLL);
  sub_401C58(a1, s);
  if ( !strcmp((const char *)s, "GET") )
    sub_40213B(a1, s);
  if ( !strcmp((const char *)s, "POST") )
    sub_40273E(a1, s);
  free(s);
}

/*
 * func-name: sub_403105
 * func-address: 0x403105
 * callers: 0x40213b
 * callees: 0x4013b0, 0x4013e0, 0x401430
 */

unsigned __int64 __fastcall sub_403105(int a1, const char *a2, const char *a3)
{
  int v4; // [rsp+2Ch] [rbp-214h]
  char s[520]; // [rsp+30h] [rbp-210h] BYREF
  unsigned __int64 v6; // [rsp+238h] [rbp-8h]

  v6 = __readfsqword(0x28u);
  if ( a3 )
    v4 = snprintf(
           s,
           0x200uLL,
           "HTTP/1.1 302 Found\r\nLocation: %s\r\nSet-Cookie: %s\r\nContent-Length: 0\r\nConnection: close\r\n\r\n",
           a2,
           a3);
  else
    v4 = snprintf(
           s,
           0x200uLL,
           "HTTP/1.1 302 Found\r\nLocation: %s\r\nContent-Length: 0\r\nConnection: close\r\n\r\n",
           a2);
  write(a1, s, v4);
  return v6 - __readfsqword(0x28u);
}

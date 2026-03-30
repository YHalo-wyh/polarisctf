/*
 * func-name: sub_40326A
 * func-address: 0x40326a
 * callers: 0x40347c
 * callees: 0x401310, 0x401350, 0x401440, 0x4014d0, 0x4014f0, 0x401b20
 */

unsigned __int64 __fastcall sub_40326A(char *a1, __int64 a2)
{
  unsigned __int64 result; // rax
  int i; // [rsp+18h] [rbp-48h]
  char *v4; // [rsp+20h] [rbp-40h]
  char *s1; // [rsp+28h] [rbp-38h]
  char *j; // [rsp+30h] [rbp-30h]
  _BYTE *s; // [rsp+48h] [rbp-18h]

  result = (unsigned __int64)a1;
  v4 = a1;
  for ( i = 0; i <= 19; ++i )
  {
    result = (unsigned __int8)*v4;
    if ( (_BYTE)result == 10 )
      break;
    for ( s1 = v4; *s1 != 38 && strncmp(s1, "\r\n", 2uLL); ++s1 )
      ;
    for ( j = v4; *j != 61 && strncmp(j, "\r\n", 2uLL); ++j )
      ;
    if ( *j == 61 )
    {
      s = malloc(0x1000uLL);
      memset(s, 0, 0x1000uLL);
      memcpy(s, v4, (int)j - (int)v4);
      *(_QWORD *)(8208LL * i + a2 + 4096) = sub_401B20((_BYTE *)(8208LL * i + a2), s);
      memcpy(s, j + 1, (int)s1 - ((int)j + 1));
      *(_QWORD *)(8208LL * i + a2 + 8200) = sub_401B20((_BYTE *)(8208LL * i + a2 + 4104), s);
      free(s);
    }
    result = (unsigned __int64)(s1 + 1);
    v4 = s1 + 1;
  }
  return result;
}

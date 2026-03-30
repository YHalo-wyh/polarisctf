/*
 * func-name: sub_401C58
 * func-address: 0x401c58
 * callers: 0x401a24
 * callees: 0x401340, 0x4013e0, 0x401410, 0x401440, 0x401460, 0x401470, 0x401490, 0x4014d0, 0x4014f0, 0x401500, 0x401580, 0x4015c0
 */

unsigned __int64 __fastcall sub_401C58(int a1, __int64 a2)
{
  size_t v2; // rax
  char *haystack; // [rsp+10h] [rbp-1070h]
  char *nptr; // [rsp+18h] [rbp-1068h]
  char *v6; // [rsp+28h] [rbp-1058h]
  char *v7; // [rsp+30h] [rbp-1050h]
  char *v8; // [rsp+30h] [rbp-1050h]
  char *v9; // [rsp+40h] [rbp-1040h]
  const char *s; // [rsp+50h] [rbp-1030h]
  char *v11; // [rsp+58h] [rbp-1028h]
  char *v12; // [rsp+68h] [rbp-1018h]
  _QWORD buf[513]; // [rsp+70h] [rbp-1010h] BYREF
  unsigned __int64 v14; // [rsp+1078h] [rbp-8h]

  v14 = __readfsqword(0x28u);
  memset(buf, 0, 0x1000uLL);
  read(a1, buf, 0xFFFuLL);
  v6 = strstr((const char *)buf, "\r\n\r\n");
  v7 = strstr((const char *)buf, "\r\n");
  *v7 = 0;
  __isoc99_sscanf();
  for ( haystack = v7 + 2; haystack < v6; haystack = v8 + 2 )
  {
    v8 = strstr(haystack, "\r\n");
    *v8 = 0;
    v9 = strchr(haystack, 58);
    if ( v9 )
    {
      *v9 = 0;
      for ( nptr = v9 + 1; *nptr == 32; ++nptr )
        ;
      if ( *(int *)(a2 + 288) <= 31 )
      {
        strncpy((char *)(320LL * *(int *)(a2 + 288) + 288 + a2 + 4), haystack, 0x3FuLL);
        strncpy((char *)(320LL * *(int *)(a2 + 288) + 352 + a2 + 4), nptr, 0xFFuLL);
        ++*(_DWORD *)(a2 + 288);
      }
      if ( !strcmp(haystack, "Content-Length") )
        *(_QWORD *)(a2 + 10864) = atoi(nptr);
      if ( !strcmp(haystack, "Cookie") )
      {
        s = strstr(nptr, "token");
        if ( s )
        {
          v11 = strchr(s, 61);
          *v11 = 0;
          v12 = strchr(v11 + 1, 59);
          if ( v12 )
            *v12 = 0;
          strncpy((char *)(a2 + 10532), s, 0x3FuLL);
          strncpy((char *)(a2 + 10596), v11 + 1, 0xFFuLL);
        }
      }
    }
  }
  if ( !strcmp((const char *)a2, "POST") )
  {
    *(_QWORD *)(a2 + 10856) = malloc(*(_QWORD *)(a2 + 10864) + 3LL);
    v2 = malloc_usable_size(*(void **)(a2 + 10856));
    memset(*(void **)(a2 + 10856), 0, v2);
    memcpy(*(void **)(a2 + 10856), v6 + 4, *(_QWORD *)(a2 + 10864));
    memcpy((void *)(*(_QWORD *)(a2 + 10856) + *(_QWORD *)(a2 + 10864)), "\r\n", 2uLL);
  }
  return v14 - __readfsqword(0x28u);
}

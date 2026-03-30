/*
 * func-name: sub_402C9E
 * func-address: 0x402c9e
 * callers: 0x40273e, 0x402bdb
 * callees: 0x4013b0, 0x4013d0, 0x4013e0, 0x401590
 */

unsigned __int64 __fastcall sub_402C9E(int a1, int a2, const char *a3, const char *a4)
{
  size_t v4; // rax
  size_t v5; // rax
  int v9; // [rsp+18h] [rbp-1048h]
  int v10; // [rsp+2Ch] [rbp-1034h]
  _QWORD v11[4]; // [rsp+30h] [rbp-1030h] BYREF
  char buf[4104]; // [rsp+50h] [rbp-1010h] BYREF
  unsigned __int64 v13; // [rsp+1058h] [rbp-8h]

  v9 = a2;
  v13 = __readfsqword(0x28u);
  memset(buf, 0, 0x1000uLL);
  memset(v11, 0, sizeof(v11));
  if ( a2 == 200 )
  {
    strcpy((char *)v11, "OK");
  }
  else if ( a2 == 404 )
  {
    strcpy((char *)v11, "Not Found");
  }
  else
  {
    strcpy((char *)v11, "Internal Server Error");
    v9 = 500;
  }
  v4 = strlen(a4);
  v10 = sprintf(
          buf,
          "HTTP/1.1 %d %s\r\nContent-Type: %s\r\nContent-Length: %zu\r\nConnection: close\r\n\r\n",
          v9,
          (const char *)v11,
          a3,
          v4);
  write(a1, buf, v10);
  v5 = strlen(a4);
  write(a1, a4, v5);
  return v13 - __readfsqword(0x28u);
}

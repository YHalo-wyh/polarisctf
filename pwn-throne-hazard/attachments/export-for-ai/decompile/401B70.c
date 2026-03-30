/*
 * func-name: sub_401B70
 * func-address: 0x401b70
 * callers: 0x4012b0
 * callees: 0x401170, 0x401190, 0x4011c0, 0x401250, 0x401290, 0x401aa0
 */

unsigned __int64 __fastcall sub_401B70(__int64 a1, unsigned __int64 a2, unsigned __int64 a3)
{
  int *v3; // rbp
  unsigned __int64 result; // rax
  char *endptr; // [rsp+18h] [rbp-90h] BYREF
  char nptr[72]; // [rsp+20h] [rbp-88h] BYREF
  unsigned __int64 v8; // [rsp+68h] [rbp-40h]

  v8 = __readfsqword(0x28u);
  endptr = 0LL;
  v3 = __errno_location();
  while ( 1 )
  {
    __printf_chk(1LL, "%s");
    sub_401AA0((__int64)nptr, 64LL);
    *v3 = 0;
    result = strtoul(nptr, &endptr, 0);
    if ( !*v3 && endptr != nptr && !*endptr && result >= a2 && result <= a3 )
      break;
    puts("invalid input");
  }
  return result;
}

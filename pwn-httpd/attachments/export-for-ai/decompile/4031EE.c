/*
 * func-name: sub_4031EE
 * func-address: 0x4031ee
 * callers: 0x40213b, 0x40273e
 * callees: 0x401340, 0x401370, 0x401410, 0x401440
 */

char *__fastcall sub_4031EE(const char *a1, void *a2)
{
  char *v3; // [rsp+18h] [rbp-8h]

  v3 = strchr(a1, 63);
  memset(a2, 0, 8uLL);
  if ( v3 )
    return strncpy((char *)a2, a1, v3 - a1);
  else
    return strcpy((char *)a2, a1);
}

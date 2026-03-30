/*
 * func-name: sub_403011
 * func-address: 0x403011
 * callers: 0x40273e
 * callees: 0x401310, 0x4013e0, 0x401490, 0x40347c, 0x4034da
 */

__int64 __fastcall sub_403011(__int64 a1, const char *a2)
{
  __int64 v3; // [rsp+18h] [rbp-18h] BYREF
  void *ptr; // [rsp+20h] [rbp-10h]
  unsigned __int64 v5; // [rsp+28h] [rbp-8h]

  v5 = __readfsqword(0x28u);
  ptr = (void *)sub_40347C(a1);
  if ( !(unsigned int)sub_4034DA("username", ptr, &v3) )
    return 0LL;
  if ( strcmp((const char *)(v3 + 4104), a2) )
    return 0LL;
  if ( !(unsigned int)sub_4034DA("password", ptr, &v3) )
    return 0LL;
  if ( strcmp((const char *)(v3 + 4104), a2 + 32) )
    return 0LL;
  free(ptr);
  return 1LL;
}

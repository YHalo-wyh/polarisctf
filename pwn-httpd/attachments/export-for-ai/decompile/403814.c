/*
 * func-name: sub_403814
 * func-address: 0x403814
 * callers: 0x40273e
 * callees: 0x401310, 0x4013e0, 0x401490, 0x4014d0, 0x40347c, 0x4034da
 */

__int64 __fastcall sub_403814(__int64 a1, __int64 a2)
{
  __int64 v3; // [rsp+10h] [rbp-20h] BYREF
  __int64 v4; // [rsp+18h] [rbp-18h] BYREF
  void *ptr; // [rsp+20h] [rbp-10h]
  unsigned __int64 v6; // [rsp+28h] [rbp-8h]

  v6 = __readfsqword(0x28u);
  ptr = sub_40347C(a1);
  if ( !(unsigned int)sub_4034DA("new_password", (__int64)ptr, &v3) )
    return 0LL;
  if ( !(unsigned int)sub_4034DA("confirm_password", (__int64)ptr, &v4) )
    return 0LL;
  if ( strcmp((const char *)(v3 + 4104), (const char *)(v4 + 4104)) )
    return 0LL;
  memcpy((void *)(a2 + 32), (const void *)(v3 + 4104), *(_QWORD *)(v3 + 8200));
  free(ptr);
  return 1LL;
}

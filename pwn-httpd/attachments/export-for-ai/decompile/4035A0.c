/*
 * func-name: sub_4035A0
 * func-address: 0x4035a0
 * callers: 0x40273e
 * callees: 0x4013e0, 0x4014d0, 0x40347c, 0x4034da
 */

__int64 __fastcall sub_4035A0(__int64 a1, _QWORD *a2)
{
  __int64 v3; // rbx
  __int64 v4; // rbx
  __int64 v5; // rbx
  __int64 v6; // rbx
  __int64 v7; // rbx
  __int64 v8; // rbx
  __int64 v9; // rbx
  __int64 v10; // rbx
  __int64 v11; // [rsp+10h] [rbp-B0h] BYREF
  void *v12; // [rsp+18h] [rbp-A8h]
  _QWORD dest[4]; // [rsp+20h] [rbp-A0h] BYREF
  _QWORD v14[4]; // [rsp+40h] [rbp-80h] BYREF
  _QWORD v15[4]; // [rsp+60h] [rbp-60h] BYREF
  _QWORD v16[7]; // [rsp+80h] [rbp-40h] BYREF

  v16[5] = __readfsqword(0x28u);
  v12 = sub_40347C(a1);
  if ( !(unsigned int)sub_4034DA("route_name", (__int64)v12, &v11) )
    return 0LL;
  memcpy(dest, (const void *)(v11 + 4104), *(_QWORD *)(v11 + 8200));
  if ( !(unsigned int)sub_4034DA("ip", (__int64)v12, &v11) )
    return 0LL;
  memcpy(v14, (const void *)(v11 + 4104), *(_QWORD *)(v11 + 8200));
  if ( !(unsigned int)sub_4034DA("subnet_mask", (__int64)v12, &v11) )
    return 0LL;
  memcpy(v15, (const void *)(v11 + 4104), *(_QWORD *)(v11 + 8200));
  if ( !(unsigned int)sub_4034DA("gateway", (__int64)v12, &v11) )
    return 0LL;
  memcpy(v16, (const void *)(v11 + 4104), *(_QWORD *)(v11 + 8200));
  v3 = dest[1];
  a2[16] = dest[0];
  a2[17] = v3;
  v4 = dest[3];
  a2[18] = dest[2];
  a2[19] = v4;
  v5 = v14[1];
  a2[20] = v14[0];
  a2[21] = v5;
  v6 = v14[3];
  a2[22] = v14[2];
  a2[23] = v6;
  v7 = v15[1];
  a2[24] = v15[0];
  a2[25] = v7;
  v8 = v15[3];
  a2[26] = v15[2];
  a2[27] = v8;
  v9 = v16[1];
  a2[28] = v16[0];
  a2[29] = v9;
  v10 = v16[3];
  a2[30] = v16[2];
  a2[31] = v10;
  return 1LL;
}

/*
 * func-name: sub_40273E
 * func-address: 0x40273e
 * callers: 0x401a24
 * callees: 0x401350, 0x4013e0, 0x401490, 0x402bdb, 0x402c9e, 0x402faf, 0x403011, 0x4031ee, 0x4035a0, 0x403814
 */

unsigned __int64 __fastcall sub_40273E(unsigned int a1, __int64 a2)
{
  char s1[8]; // [rsp+50h] [rbp-110h] BYREF
  __int64 v4; // [rsp+58h] [rbp-108h]
  __int64 v5; // [rsp+60h] [rbp-100h]
  __int64 v6; // [rsp+68h] [rbp-F8h]
  __int64 v7; // [rsp+70h] [rbp-F0h]
  __int64 v8; // [rsp+78h] [rbp-E8h]
  __int64 v9; // [rsp+80h] [rbp-E0h]
  __int64 v10; // [rsp+88h] [rbp-D8h]
  __int64 v11; // [rsp+90h] [rbp-D0h]
  __int64 v12; // [rsp+98h] [rbp-C8h]
  __int64 v13; // [rsp+A0h] [rbp-C0h]
  __int64 v14; // [rsp+A8h] [rbp-B8h]
  __int64 v15; // [rsp+B0h] [rbp-B0h]
  __int64 v16; // [rsp+B8h] [rbp-A8h]
  __int64 v17; // [rsp+C0h] [rbp-A0h]
  __int64 v18; // [rsp+C8h] [rbp-98h]
  __int64 v19; // [rsp+D0h] [rbp-90h]
  __int64 v20; // [rsp+D8h] [rbp-88h]
  __int64 v21; // [rsp+E0h] [rbp-80h]
  __int64 v22; // [rsp+E8h] [rbp-78h]
  __int64 v23; // [rsp+F0h] [rbp-70h]
  __int64 v24; // [rsp+F8h] [rbp-68h]
  __int64 v25; // [rsp+100h] [rbp-60h]
  __int64 v26; // [rsp+108h] [rbp-58h]
  __int64 v27; // [rsp+110h] [rbp-50h]
  __int64 v28; // [rsp+118h] [rbp-48h]
  __int64 v29; // [rsp+120h] [rbp-40h]
  __int64 v30; // [rsp+128h] [rbp-38h]
  __int64 v31; // [rsp+130h] [rbp-30h]
  __int64 v32; // [rsp+138h] [rbp-28h]
  __int64 v33; // [rsp+140h] [rbp-20h]
  __int64 v34; // [rsp+148h] [rbp-18h]
  unsigned __int64 v35; // [rsp+158h] [rbp-8h]

  v35 = __readfsqword(0x28u);
  *(_QWORD *)s1 = 0LL;
  v4 = 0LL;
  v5 = 0LL;
  v6 = 0LL;
  v7 = 0LL;
  v8 = 0LL;
  v9 = 0LL;
  v10 = 0LL;
  v11 = 0LL;
  v12 = 0LL;
  v13 = 0LL;
  v14 = 0LL;
  v15 = 0LL;
  v16 = 0LL;
  v17 = 0LL;
  v18 = 0LL;
  v19 = 0LL;
  v20 = 0LL;
  v21 = 0LL;
  v22 = 0LL;
  v23 = 0LL;
  v24 = 0LL;
  v25 = 0LL;
  v26 = 0LL;
  v27 = 0LL;
  v28 = 0LL;
  v29 = 0LL;
  v30 = 0LL;
  v31 = 0LL;
  v32 = 0LL;
  v33 = 0LL;
  v34 = 0LL;
  sub_4031EE(a2 + 16, s1);
  if ( !strncmp(s1, "/login", 6uLL) )
  {
    if ( (unsigned int)sub_403011(a2, qword_4061F0) )
    {
      sub_402C9E(a1, 200LL, "application/json", "{\"authLogin\" : 1}");
      return v35 - __readfsqword(0x28u);
    }
LABEL_16:
    sub_402C9E(a1, 200LL, "application/json", "{\"authLogin\" : 0}");
    return v35 - __readfsqword(0x28u);
  }
  if ( !strcmp(s1, "/resetPasswd") )
  {
    if ( !(unsigned int)sub_402FAF(a2) )
      goto LABEL_16;
    if ( (unsigned int)sub_403814(a2, qword_4061F0) )
      sub_402C9E(a1, 200LL, "application/json", "{\"reset\" : 1}");
    else
      sub_402C9E(a1, 200LL, "application/json", "{\"reset\" : 0}");
  }
  else
  {
    if ( strcmp(s1, "/config") )
    {
      sub_402BDB(a1, "404.html", 404LL);
      return v35 - __readfsqword(0x28u);
    }
    if ( !(unsigned int)sub_402FAF(a2) )
      goto LABEL_16;
    if ( (unsigned int)sub_4035A0(a2, qword_4061F0) )
      sub_402C9E(a1, 200LL, "application/json", "{\"setInfo\" : 1}");
    else
      sub_402C9E(a1, 200LL, "application/json", "{\"setInfo\" : 0}");
  }
  return v35 - __readfsqword(0x28u);
}

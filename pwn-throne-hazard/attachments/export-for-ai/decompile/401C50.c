/*
 * func-name: sub_401C50
 * func-address: 0x401c50
 * callers: 0x4012b0
 * callees: 0x4011c0, 0x401250, 0x401aa0
 */

unsigned __int64 __fastcall sub_401C50(__int64 a1, _QWORD *a2, unsigned __int64 a3, unsigned __int64 *a4)
{
  unsigned __int64 v7; // rax
  unsigned __int64 v8; // rsi
  unsigned __int64 v9; // rdx
  _QWORD v11[33]; // [rsp+0h] [rbp-138h] BYREF
  unsigned __int64 v12; // [rsp+108h] [rbp-30h]

  v12 = __readfsqword(0x28u);
  __printf_chk(1LL, "%s");
  v7 = sub_401AA0((__int64)v11, 256LL);
  v8 = (unsigned __int64)(a2 + 1);
  *a2 = 0LL;
  v9 = v7;
  if ( v7 >= a3 )
    v9 = a3 - 1;
  *(_QWORD *)((char *)a2 + (unsigned int)a3 - 8) = 0LL;
  memset(
    (void *)((unsigned __int64)(a2 + 1) & 0xFFFFFFFFFFFFFFF8LL),
    0,
    8LL * (((unsigned int)a3 + (_DWORD)a2 - (((_DWORD)a2 + 8) & 0xFFFFFFF8)) >> 3));
  if ( (unsigned int)v9 >= 8 )
  {
    *a2 = v11[0];
    *(_QWORD *)((char *)a2 + (unsigned int)v9 - 8) = *(_QWORD *)((char *)&v11[-1] + (unsigned int)v9);
    qmemcpy(
      (void *)(v8 & 0xFFFFFFFFFFFFFFF8LL),
      (const void *)((char *)v11 - ((char *)a2 - (v8 & 0xFFFFFFFFFFFFFFF8LL))),
      8 * (((unsigned int)v9 + (unsigned int)a2 - (v8 & 0xFFFFFFF8)) >> 3));
  }
  else if ( (v9 & 4) != 0 )
  {
    *(_DWORD *)a2 = v11[0];
    *(_DWORD *)((char *)a2 + (unsigned int)v9 - 4) = *(_DWORD *)((char *)v11 + (unsigned int)v9 - 4);
  }
  else if ( (_DWORD)v9 )
  {
    *(_BYTE *)a2 = v11[0];
    if ( (v9 & 2) != 0 )
      *(_WORD *)((char *)a2 + (unsigned int)v9 - 2) = *(_WORD *)((char *)v11 + (unsigned int)v9 - 2);
  }
  if ( a4 )
    *a4 = v9;
  return v12 - __readfsqword(0x28u);
}

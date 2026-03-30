/*
 * func-name: sub_6D00
 * func-address: 0x6d00
 * callers: 0x6e90
 * callees: 0x2410, 0x2480, 0x24b0, 0x24c0, 0x25e0
 */

void __fastcall sub_6D00(__int64 a1, _BYTE *a2, __int64 *a3)
{
  _BYTE *v4; // r12
  _BYTE *v5; // r13
  unsigned __int64 v6; // rax
  signed __int64 v7; // rdx
  unsigned __int64 v8; // rcx
  unsigned __int64 v9; // rbx
  char *v10; // rcx
  __int64 v11; // rax
  char *v12; // r8
  signed __int64 v13; // r12
  char *v14; // r15
  unsigned __int64 v15; // rbx
  __int64 v16; // rax
  char *v17; // rax
  bool v18; // cf
  unsigned __int64 v19; // rax
  __int64 v20; // rbx
  char *desta; // [rsp+0h] [rbp-48h]
  char *dest; // [rsp+0h] [rbp-48h]
  char *destb; // [rsp+0h] [rbp-48h]
  char *v24; // [rsp+8h] [rbp-40h]

  v4 = *(_BYTE **)(a1 + 8);
  v5 = *(_BYTE **)a1;
  v6 = (__int64)&v4[-*(_QWORD *)a1] >> 3;
  if ( v6 == 0xFFFFFFFFFFFFFFFLL )
    std::__throw_length_error("vector::_M_realloc_insert");
  v7 = a2 - v5;
  if ( v5 == v4 )
  {
    v18 = __CFADD__(v6, 1LL);
    v19 = v6 + 1;
    if ( !v18 )
    {
      v20 = 0xFFFFFFFFFFFFFFFLL;
      if ( v19 <= 0xFFFFFFFFFFFFFFFLL )
        v20 = v19;
      v15 = 8 * v20;
      goto LABEL_12;
    }
    goto LABEL_11;
  }
  v8 = 2 * v6;
  if ( 2 * v6 < v6 )
  {
LABEL_11:
    v15 = 0x7FFFFFFFFFFFFFF8LL;
    goto LABEL_12;
  }
  if ( !v8 )
  {
    v9 = 0LL;
    v10 = 0LL;
    goto LABEL_6;
  }
  if ( v8 > 0xFFFFFFFFFFFFFFFLL )
    v8 = 0xFFFFFFFFFFFFFFFLL;
  v15 = 8 * v8;
LABEL_12:
  v16 = operator new(v15);
  v7 = a2 - v5;
  v10 = (char *)v16;
  v9 = v16 + v15;
LABEL_6:
  v11 = *a3;
  v12 = &v10[v7 + 8];
  v13 = v4 - a2;
  v14 = &v12[v13];
  *(_QWORD *)&v10[v7] = v11;
  if ( v7 > 0 )
  {
    dest = &v10[v7 + 8];
    v17 = (char *)memmove(v10, v5, v7);
    v10 = v17;
    if ( v13 > 0 )
    {
      v24 = v17;
      memcpy(dest, a2, v13);
      v10 = v24;
    }
    goto LABEL_15;
  }
  if ( v13 > 0 )
  {
    desta = v10;
    memcpy(v12, a2, v13);
    v10 = desta;
  }
  if ( v5 )
  {
LABEL_15:
    destb = v10;
    operator delete(v5, *(_QWORD *)(a1 + 16) - (_QWORD)v5);
    v10 = destb;
  }
  *(_QWORD *)a1 = v10;
  *(_QWORD *)(a1 + 8) = v14;
  *(_QWORD *)(a1 + 16) = v9;
}

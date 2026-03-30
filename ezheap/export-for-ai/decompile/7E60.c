/*
 * func-name: sub_7E60
 * func-address: 0x7e60
 * callers: 0x7fb0
 * callees: 0x2410, 0x2480, 0x24b0, 0x24c0, 0x25e0
 */

void __fastcall sub_7E60(__int64 a1, _BYTE *a2, _BYTE *a3)
{
  _BYTE *v3; // r15
  unsigned __int64 v4; // rax
  unsigned __int64 v5; // r13
  signed __int64 v6; // r12
  _BYTE *v7; // rax
  size_t v8; // rdx
  _BYTE *v9; // rbx
  char *v10; // rcx
  char *v11; // r12
  unsigned __int64 v12; // rsi
  size_t n; // [rsp+0h] [rbp-48h]
  size_t na; // [rsp+0h] [rbp-48h]

  v3 = *(_BYTE **)a1;
  v4 = *(_QWORD *)(a1 + 8) - *(_QWORD *)a1;
  if ( v4 == 0x7FFFFFFFFFFFFFFFLL )
    std::__throw_length_error("vector::_M_realloc_insert");
  v5 = 1LL;
  if ( v4 )
  {
    v5 = 0x7FFFFFFFFFFFFFFFLL;
    if ( 2 * v4 <= 0x7FFFFFFFFFFFFFFFLL )
      v5 = 2 * v4;
    if ( 2 * v4 < v4 )
      v5 = 0x7FFFFFFFFFFFFFFFLL;
  }
  v6 = *(_QWORD *)(a1 + 8) - (_QWORD)a2;
  n = a2 - v3;
  v7 = (_BYTE *)operator new(v5);
  v8 = n;
  v9 = v7;
  v7[n] = *a3;
  if ( a2 - v3 > 0 )
  {
    na = n + 1;
    memmove(v7, v3, v8);
    if ( v6 <= 0 )
    {
      v11 = &v9[na + v6];
      v12 = *(_QWORD *)(a1 + 16) - (_QWORD)v3;
      goto LABEL_15;
    }
    v11 = (char *)memcpy(&v9[na], a2, v6) + v6;
    goto LABEL_14;
  }
  v10 = &v7[n + 1];
  if ( v6 > 0 )
  {
    v11 = (char *)memcpy(v10, a2, v6) + v6;
    if ( !v3 )
      goto LABEL_10;
    goto LABEL_14;
  }
  v11 = &v10[v6];
  if ( v3 )
  {
LABEL_14:
    v12 = *(_QWORD *)(a1 + 16) - (_QWORD)v3;
LABEL_15:
    operator delete(v3, v12);
  }
LABEL_10:
  *(_QWORD *)a1 = v9;
  *(_QWORD *)(a1 + 8) = v11;
  *(_QWORD *)(a1 + 16) = &v9[v5];
}

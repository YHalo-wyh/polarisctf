/*
 * func-name: sub_2D30
 * func-address: 0x2d30
 * callers: 0x3910, 0x3df0, 0x4230, 0x4d60, 0x55b0, 0x6e90, 0x7fb0, 0x8a60
 * callees: 0x2370, 0x2380, 0x2480, 0x24c0, 0x24e0, 0x2570, 0x2590, 0x2630
 */

__int64 __fastcall sub_2D30(__int64 *a1)
{
  unsigned __int64 v1; // rbp
  __int64 v2; // r12
  unsigned __int64 v3; // rbx
  unsigned __int64 i; // r15
  size_t v5; // rdx
  size_t v6; // rbp
  _BYTE *v7; // r13
  _QWORD *v8; // rax
  _QWORD *v10; // rdi
  _QWORD *v11; // [rsp+20h] [rbp-68h]
  size_t v12; // [rsp+28h] [rbp-60h]
  _QWORD v13[11]; // [rsp+30h] [rbp-58h] BYREF

  v1 = a1[1];
  v13[3] = __readfsqword(0x28u);
  if ( v1 )
  {
    v2 = *a1;
    v3 = 0LL;
    while ( isspace(*(unsigned __int8 *)(v2 + v3)) )
    {
      if ( ++v3 == v1 )
      {
        i = v3;
        goto LABEL_9;
      }
    }
    for ( i = v1; v3 < i && isspace(*(unsigned __int8 *)(v2 + i - 1)); --i )
      ;
LABEL_9:
    v5 = i - v3;
    if ( v1 < v3 )
      std::__throw_out_of_range_fmt("%s: __pos (which is %zu) > this->size() (which is %zu)");
  }
  else
  {
    v3 = 0LL;
    v5 = 0LL;
  }
  v6 = v1 - v3;
  v11 = v13;
  v7 = (_BYTE *)(v3 + *a1);
  if ( v6 > v5 )
    v6 = v5;
  if ( v6 > 0xF )
  {
    v11 = (_QWORD *)std::string::_M_create();
    v10 = v11;
    v13[0] = v6;
  }
  else
  {
    if ( v6 == 1 )
    {
      LOBYTE(v13[0]) = *v7;
LABEL_15:
      v8 = v13;
      goto LABEL_16;
    }
    if ( !v6 )
      goto LABEL_15;
    v10 = v13;
  }
  memcpy(v10, v7, v6);
  v8 = v11;
LABEL_16:
  v12 = v6;
  *((_BYTE *)v8 + v6) = 0;
  if ( v12 )
  {
    *__errno_location() = 0;
    __isoc23_strtoull();
  }
  if ( v11 != v13 )
    operator delete(v11, v13[0] + 1LL);
  return 0LL;
}

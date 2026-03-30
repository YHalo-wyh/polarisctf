/*
 * func-name: sub_6720
 * func-address: 0x6720
 * callers: 0x27a0
 * callees: none
 */

void __fastcall sub_6720(__int64 a1)
{
  if ( *(_QWORD *)a1 )
    operator delete(*(void **)a1, *(_QWORD *)(a1 + 16) - *(_QWORD *)a1);
}

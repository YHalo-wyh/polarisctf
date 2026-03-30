/*
 * func-name: sub_4200
 * func-address: 0x4200
 * callers: 0x7fb0
 * callees: none
 */

void __fastcall sub_4200(__int64 a1)
{
  if ( *(_QWORD *)a1 )
    operator delete(*(void **)a1, *(_QWORD *)(a1 + 16) - *(_QWORD *)a1);
}

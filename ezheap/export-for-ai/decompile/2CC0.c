/*
 * func-name: sub_2CC0
 * func-address: 0x2cc0
 * callers: 0x27a0, 0x36a0, 0x4230, 0x4d60, 0x6e90
 * callees: 0x2360, 0x2500, 0x2530
 */

__int64 __fastcall sub_2CC0(std::ostream *this)
{
  _BYTE *v1; // rbp
  char v2; // si
  std::ostream *v3; // rax
  __int64 (__fastcall *v5)(); // rax

  v1 = *(_BYTE **)((char *)this + *(_QWORD *)(*(_QWORD *)this - 24LL) + 240);
  if ( !v1 )
    std::__throw_bad_cast();
  if ( v1[56] )
  {
    v2 = v1[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v2 = 10;
    v5 = *(__int64 (__fastcall **)())(*(_QWORD *)v1 + 48LL);
    if ( v5 != std::ctype<char>::do_widen )
      v2 = ((__int64 (__fastcall *)(_BYTE *, __int64))v5)(v1, 10LL);
  }
  v3 = (std::ostream *)std::ostream::put(this, v2);
  return std::ostream::flush(v3);
}

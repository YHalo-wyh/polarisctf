/*
 * func-name: task_runner_mul_ack
 * func-address: 0x2f40
 * callers: 0x36a0, 0x3df0
 * callees: 0x2360, 0x24f0, 0x2500, 0x2530
 */

void __fastcall task_runner_mul_ack(__int64 a1)
{
  _BYTE *v1; // rbx
  char v2; // si
  std::ostream *v3; // rax
  __int64 (__fastcall *v4)(); // rax

  if ( a1 )
  {
    *(_QWORD *)(a1 + 16) = 3LL * *(_QWORD *)(a1 + 8);
    std::__ostream_insert<char,std::char_traits<char>>();
    v1 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
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
      v4 = *(__int64 (__fastcall **)())(*(_QWORD *)v1 + 48LL);
      if ( v4 != std::ctype<char>::do_widen )
        v2 = ((__int64 (__fastcall *)(_BYTE *, __int64))v4)(v1, 10LL);
    }
    v3 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v2);
    std::ostream::flush(v3);
  }
}

/*
 * func-name: task_runner_log_ack
 * func-address: 0x30a0
 * callers: 0x36a0, 0x3df0
 * callees: 0x2360, 0x2400, 0x2460, 0x24f0, 0x2500, 0x2530
 */

__int64 __fastcall task_runner_log_ack(const char *a1)
{
  std::ostream *v1; // rbp
  _BYTE *v2; // rbx
  char v3; // si
  std::ostream *v4; // rax
  __int64 (__fastcall *v6)(); // rax

  if ( a1 )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    *(_DWORD *)((char *)&std::cout[3] + *(_QWORD *)(std::cout[0] - 24LL)) = *(_DWORD *)((_BYTE *)&std::cout[3]
                                                                                      + *(_QWORD *)(std::cout[0] - 24LL)) & 0xFFFFFFB5 | 2;
    std::ostream::_M_insert<unsigned long>();
    std::__ostream_insert<char,std::char_traits<char>>();
    v1 = (std::ostream *)std::ostream::_M_insert<unsigned long>();
    std::__ostream_insert<char,std::char_traits<char>>();
    strlen(a1 + 40);
    std::__ostream_insert<char,std::char_traits<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    v2 = *(_BYTE **)((char *)v1 + *(_QWORD *)(*(_QWORD *)v1 - 24LL) + 240);
    if ( v2 )
      goto LABEL_3;
LABEL_9:
    std::__throw_bad_cast();
  }
  v1 = (std::ostream *)std::cout;
  std::__ostream_insert<char,std::char_traits<char>>();
  v2 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
  if ( !v2 )
    goto LABEL_9;
LABEL_3:
  if ( v2[56] )
  {
    v3 = v2[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v3 = 10;
    v6 = *(__int64 (__fastcall **)())(*(_QWORD *)v2 + 48LL);
    if ( v6 != std::ctype<char>::do_widen )
      v3 = ((__int64 (__fastcall *)(_BYTE *, __int64))v6)(v2, 10LL);
  }
  v4 = (std::ostream *)std::ostream::put(v1, v3);
  return std::ostream::flush(v4);
}

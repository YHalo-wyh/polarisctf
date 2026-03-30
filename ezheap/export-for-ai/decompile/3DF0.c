/*
 * func-name: dispatch_async_task
 * func-address: 0x3df0
 * callers: 0x8a60
 * callees: 0x2360, 0x2470, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2620, 0x2650, 0x2d30
 */

// Dispatches desc->handler(desc->ctx) for task_id 0..7. strict_policy only allows the three built-in task runners.
__int64 __fastcall dispatch_async_task(__int64 a1)
{
  _BYTE *v1; // r14
  _QWORD *v2; // rax
  _BYTE *v3; // rbx
  char v4; // si
  std::ostream *v5; // rax
  __int64 (__fastcall *v7)(__int64, unsigned int); // rax
  __int64 v8; // rcx
  void (__fastcall *v9)(_QWORD); // rax
  _BYTE *v10; // rbx
  char v11; // si
  std::ostream *v12; // rdi
  __int64 (__fastcall *v13)(__int64, unsigned int); // rax
  __int64 (__fastcall *v14)(__int64, unsigned int); // rax
  void *v15[2]; // [rsp+10h] [rbp-68h] BYREF
  _QWORD v16[3]; // [rsp+20h] [rbp-58h] BYREF
  unsigned __int64 v17; // [rsp+38h] [rbp-40h]

  v17 = __readfsqword(0x28u);
  if ( *(_QWORD *)(a1 + 32) )
  {
    v15[0] = v16;
    v15[1] = 0LL;
    LOBYTE(v16[0]) = 0;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v1 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v1 )
      std::__throw_bad_cast();
    if ( !v1[56] )
    {
      std::ctype<char>::_M_widen_init();
      v7 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v1 + 48LL);
      if ( v7 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v7)(v1, 10LL, 10LL);
    }
    v2 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v2 + *(_QWORD *)(*v2 - 24LL) + 32) & 5) != 0 )
      goto LABEL_10;
    if ( (unsigned __int8)sub_2D30((__int64 *)v15) )
    {
      v8 = *(_QWORD *)(a1 + 40);
      if ( v8 && (v9 = *(void (__fastcall **)(_QWORD))(v8 + 24)) != 0LL )
      {
        if ( v9 == task_runner_xor_ack
          || (char *)v9 == (char *)task_runner_log_ack
          || *(_BYTE *)(*(_QWORD *)(a1 + 32) + 8LL) == 0
          || v9 == task_runner_mul_ack )
        {
          v9(*(_QWORD *)(v8 + 32));
LABEL_10:
          if ( v15[0] != v16 )
            operator delete(v15[0], v16[0] + 1LL);
          return v17 - __readfsqword(0x28u);
        }
        std::__ostream_insert<char,std::char_traits<char>>();
        v3 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
        if ( !v3 )
          std::__throw_bad_cast();
      }
      else
      {
        std::__ostream_insert<char,std::char_traits<char>>();
        v3 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
        if ( !v3 )
          std::__throw_bad_cast();
      }
    }
    else
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v3 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v3 )
        std::__throw_bad_cast();
    }
    if ( v3[56] )
    {
      v4 = v3[67];
    }
    else
    {
      std::ctype<char>::_M_widen_init();
      v4 = 10;
      v13 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v3 + 48LL);
      if ( v13 != std::ctype<char>::do_widen )
        v4 = v13((__int64)v3, 10u);
    }
    v5 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v4);
    std::ostream::flush(v5);
    goto LABEL_10;
  }
  std::__ostream_insert<char,std::char_traits<char>>();
  v10 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
  if ( !v10 )
    std::__throw_bad_cast();
  if ( v10[56] )
  {
    v11 = v10[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v11 = 10;
    v14 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v10 + 48LL);
    if ( v14 != std::ctype<char>::do_widen )
      v11 = v14((__int64)v10, 10u);
  }
  v12 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v11);
  return std::ostream::flush(v12);
}

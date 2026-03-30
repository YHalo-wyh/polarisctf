/*
 * func-name: inspect_scheduler_queue
 * func-address: 0x4870
 * callers: 0x8a60
 * callees: 0x2360, 0x2400, 0x2460, 0x2470, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2610, 0x2620, 0x3210
 */

__int64 __fastcall inspect_scheduler_queue(__int64 a1)
{
  std::ostream *v1; // rbx
  _BYTE *v2; // rbp
  char v3; // si
  std::ostream *v4; // rax
  __int64 i; // r12
  char v6; // si
  std::ostream *v7; // rax
  __int64 v8; // rbp
  std::ostream *v9; // rbx
  _BYTE *v10; // rbp
  __int64 (__fastcall *v11)(__int64, unsigned int); // rax
  __int64 (__fastcall *v12)(__int64, unsigned int); // rax
  _BYTE *v14; // rbx
  char v15; // si
  std::ostream *v16; // rdi
  __int64 (__fastcall *v17)(__int64, unsigned int); // rax
  void *v18; // [rsp+20h] [rbp-A8h] BYREF
  __int64 v19; // [rsp+30h] [rbp-98h] BYREF
  void *v20; // [rsp+40h] [rbp-88h] BYREF
  __int64 v21; // [rsp+50h] [rbp-78h] BYREF
  void *v22; // [rsp+60h] [rbp-68h] BYREF
  _QWORD v23[3]; // [rsp+70h] [rbp-58h] BYREF
  __int64 v24; // [rsp+88h] [rbp-40h]

  v24 = __readfsqword(0x28u);
  if ( *(_QWORD *)(a1 + 32) )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    sub_3210((__int64)&v22, *(_QWORD *)(a1 + 32));
    std::__ostream_insert<char,std::char_traits<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::operator<<();
    std::__ostream_insert<char,std::char_traits<char>>();
    v1 = (std::ostream *)std::ostream::operator<<();
    v2 = *(_BYTE **)((char *)v1 + *(_QWORD *)(*(_QWORD *)v1 - 24LL) + 240);
    if ( !v2 )
      std::__throw_bad_cast();
    if ( v2[56] )
    {
      v3 = v2[67];
    }
    else
    {
      std::ctype<char>::_M_widen_init();
      v3 = 10;
      v12 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v2 + 48LL);
      if ( v12 != std::ctype<char>::do_widen )
        v3 = v12((__int64)v2, 10u);
    }
    v4 = (std::ostream *)std::ostream::put(v1, v3);
    std::ostream::flush(v4);
    if ( v22 != v23 )
      operator delete(v22, v23[0] + 1LL);
    for ( i = 0LL; i != 8; ++i )
    {
      v8 = *(_QWORD *)(a1 + 8 * i + 40);
      std::__ostream_insert<char,std::char_traits<char>>();
      std::ostream::_M_insert<unsigned long>();
      std::__ostream_insert<char,std::char_traits<char>>();
      sub_3210((__int64)&v18, v8);
      std::__ostream_insert<char,std::char_traits<char>>();
      std::__ostream_insert<char,std::char_traits<char>>();
      sub_3210((__int64)&v20, *(_QWORD *)(v8 + 24));
      std::__ostream_insert<char,std::char_traits<char>>();
      std::__ostream_insert<char,std::char_traits<char>>();
      sub_3210((__int64)&v22, *(_QWORD *)(v8 + 32));
      v9 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
      std::__ostream_insert<char,std::char_traits<char>>();
      strlen((const char *)(v8 + 40));
      std::__ostream_insert<char,std::char_traits<char>>();
      std::__ostream_insert<char,std::char_traits<char>>();
      v10 = *(_BYTE **)((char *)v9 + *(_QWORD *)(*(_QWORD *)v9 - 24LL) + 240);
      if ( !v10 )
        std::__throw_bad_cast();
      if ( v10[56] )
      {
        v6 = v10[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v6 = 10;
        v11 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v10 + 48LL);
        if ( v11 != std::ctype<char>::do_widen )
          v6 = v11((__int64)v10, 10u);
      }
      v7 = (std::ostream *)std::ostream::put(v9, v6);
      std::ostream::flush(v7);
      if ( v22 != v23 )
        operator delete(v22, v23[0] + 1LL);
      if ( v20 != &v21 )
        operator delete(v20, v21 + 1);
      if ( v18 != &v19 )
        operator delete(v18, v19 + 1);
    }
    return v24 - __readfsqword(0x28u);
  }
  else
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    v14 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
    if ( !v14 )
      std::__throw_bad_cast();
    if ( v14[56] )
    {
      v15 = v14[67];
    }
    else
    {
      std::ctype<char>::_M_widen_init();
      v15 = 10;
      v17 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v14 + 48LL);
      if ( v17 != std::ctype<char>::do_widen )
        v15 = v17((__int64)v14, 10u);
    }
    v16 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v15);
    return std::ostream::flush(v16);
  }
}

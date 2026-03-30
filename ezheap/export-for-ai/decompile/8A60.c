/*
 * func-name: sub_8A60
 * func-address: 0x8a60
 * callers: 0x27a0
 * callees: 0x2360, 0x2400, 0x2470, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2540, 0x2600, 0x2620, 0x2650, 0x2d30, 0x36a0, 0x3910, 0x3df0, 0x4230, 0x4870, 0x4d60, 0x55b0, 0x5d60, 0x6e90, 0x7fb0
 */

unsigned __int64 sub_8A60()
{
  __int64 v0; // rax
  _BYTE *v1; // rbp
  _QWORD *v2; // rax
  _BYTE *v3; // rbp
  char v4; // si
  std::ostream *v5; // rax
  _BYTE *v7; // rbp
  char v8; // si
  std::ostream *v9; // rax
  __int64 (__fastcall *v10)(__int64, unsigned int); // rax
  __int64 (__fastcall *v11)(__int64, unsigned int); // rax
  __int64 (__fastcall *v12)(__int64, unsigned int); // rax
  void *v13[2]; // [rsp+20h] [rbp-68h] BYREF
  _QWORD v14[3]; // [rsp+30h] [rbp-58h] BYREF
  unsigned __int64 v15; // [rsp+48h] [rbp-40h]

  v15 = __readfsqword(0x28u);
  prctl(38, 1LL, 0LL, 0LL, 0LL);
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  while ( 1 )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    LOBYTE(v14[0]) = 0;
    v13[1] = 0LL;
    v0 = *(_QWORD *)(std::cin[0] - 24);
    v13[0] = v14;
    v1 = *(_BYTE **)((char *)&std::cin[30] + v0);
    if ( !v1 )
      std::__throw_bad_cast();
    if ( !v1[56] )
    {
      std::ctype<char>::_M_widen_init();
      v10 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v1 + 48LL);
      if ( v10 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v10)(v1, 10LL, 10LL);
    }
    v2 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v2 + *(_QWORD *)(*v2 - 24LL) + 32) & 5) != 0 )
      break;
    if ( (unsigned __int8)sub_2D30((__int64 *)v13) )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v3 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v3 )
        std::__throw_bad_cast();
      if ( v3[56] )
      {
        v4 = v3[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v4 = 10;
        v12 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v3 + 48LL);
        if ( v12 != std::ctype<char>::do_widen )
          v4 = v12((__int64)v3, 10u);
      }
      v5 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v4);
      std::ostream::flush(v5);
      break;
    }
    std::__ostream_insert<char,std::char_traits<char>>();
    v7 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
    if ( !v7 )
      std::__throw_bad_cast();
    if ( v7[56] )
    {
      v8 = v7[67];
    }
    else
    {
      std::ctype<char>::_M_widen_init();
      v8 = 10;
      v11 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v7 + 48LL);
      if ( v11 != std::ctype<char>::do_widen )
        v8 = v11((__int64)v7, 10u);
    }
    v9 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v8);
    std::ostream::flush(v9);
    if ( v13[0] != v14 )
      operator delete(v13[0], v14[0] + 1LL);
  }
  if ( v13[0] != v14 )
    operator delete(v13[0], v14[0] + 1LL);
  return v15 - __readfsqword(0x28u);
}

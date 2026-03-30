/*
 * func-name: allocate_session_tensor
 * func-address: 0x4d60
 * callers: 0x8a60
 * callees: 0x2360, 0x2430, 0x2460, 0x2470, 0x24a0, 0x24b0, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2550, 0x25d0, 0x2620, 0x2650, 0x2cc0, 0x2d30, 0x3210
 */

unsigned __int64 allocate_session_tensor()
{
  _BYTE *v0; // r14
  _QWORD *v1; // rax
  _BYTE *v2; // r14
  _QWORD *v3; // rax
  _BYTE *v4; // r14
  _QWORD *v5; // rax
  __int64 (__fastcall *v7)(__int64, unsigned int); // rax
  __int64 (__fastcall *v8)(__int64, unsigned int); // rax
  __int64 (__fastcall *v9)(__int64, unsigned int); // rax
  _BYTE *v10; // r14
  char v11; // si
  std::ostream *v12; // rax
  __int64 (__fastcall *v13)(__int64, unsigned int); // rax
  void *v14; // [rsp+40h] [rbp-108h] BYREF
  int v15[2]; // [rsp+48h] [rbp-100h]
  int v16[2]; // [rsp+50h] [rbp-F8h] BYREF
  void *v17; // [rsp+60h] [rbp-E8h] BYREF
  int v18[2]; // [rsp+68h] [rbp-E0h]
  int v19[2]; // [rsp+70h] [rbp-D8h] BYREF
  void *v20; // [rsp+80h] [rbp-C8h]
  __int64 v21; // [rsp+88h] [rbp-C0h]
  int v22[2]; // [rsp+90h] [rbp-B8h] BYREF
  int v23[2]; // [rsp+108h] [rbp-40h]

  *(_QWORD *)v23 = __readfsqword(0x28u);
  LOBYTE(v16[0]) = 0;
  v14 = v16;
  v17 = v19;
  *(_QWORD *)v15 = 0LL;
  *(_QWORD *)v18 = 0LL;
  LOBYTE(v19[0]) = 0;
  v20 = v22;
  v21 = 0LL;
  LOBYTE(v22[0]) = 0;
  std::__ostream_insert<char,std::char_traits<char>>();
  std::ostream::flush((std::ostream *)std::cout);
  v0 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
  if ( !v0 )
    std::__throw_bad_cast();
  if ( !v0[56] )
  {
    std::ctype<char>::_M_widen_init();
    v7 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v0 + 48LL);
    if ( v7 != std::ctype<char>::do_widen )
      ((void (__fastcall *)(_BYTE *, __int64, __int64))v7)(v0, 10LL, 10LL);
  }
  v1 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
  if ( (*((_BYTE *)v1 + *(_QWORD *)(*v1 - 24LL) + 32) & 5) == 0 )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v2 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v2 )
      std::__throw_bad_cast();
    if ( !v2[56] )
    {
      std::ctype<char>::_M_widen_init();
      v8 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v2 + 48LL);
      if ( v8 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v8)(v2, 10LL, 10LL);
    }
    v3 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v3 + *(_QWORD *)(*v3 - 24LL) + 32) & 5) == 0 )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      std::ostream::flush((std::ostream *)std::cout);
      v4 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
      if ( !v4 )
        std::__throw_bad_cast();
      if ( !v4[56] )
      {
        std::ctype<char>::_M_widen_init();
        v9 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v4 + 48LL);
        if ( v9 != std::ctype<char>::do_widen )
          ((void (__fastcall *)(_BYTE *, __int64, __int64))v9)(v4, 10LL, 10LL);
      }
      v5 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
      if ( (*((_BYTE *)v5 + *(_QWORD *)(*v5 - 24LL) + 32) & 5) == 0 )
      {
        if ( (unsigned __int8)sub_2D30((__int64 *)&v14) )
          sub_2D30((__int64 *)&v17);
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
          v13 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v10 + 48LL);
          if ( v13 != std::ctype<char>::do_widen )
            v11 = v13((__int64)v10, 10u);
        }
        v12 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v11);
        std::ostream::flush(v12);
      }
    }
  }
  if ( v20 != v22 )
    operator delete(v20, *(_QWORD *)v22 + 1LL);
  if ( v17 != v19 )
    operator delete(v17, *(_QWORD *)v19 + 1LL);
  if ( v14 != v16 )
    operator delete(v14, *(_QWORD *)v16 + 1LL);
  return *(_QWORD *)v23 - __readfsqword(0x28u);
}

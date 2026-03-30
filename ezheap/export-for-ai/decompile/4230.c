/*
 * func-name: register_model_artifact
 * func-address: 0x4230
 * callers: 0x8a60
 * callees: 0x2360, 0x2450, 0x2460, 0x2470, 0x24a0, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2510, 0x2520, 0x2530, 0x2620, 0x2650, 0x2cc0, 0x2d30, 0x3210
 */

unsigned __int64 __fastcall register_model_artifact(__int64 a1)
{
  _BYTE *v1; // r13
  _QWORD *v2; // rax
  _BYTE *v3; // r13
  _QWORD *v4; // rax
  void *v5; // rdi
  void *v6; // rax
  std::ostream *v7; // r14
  _BYTE *v8; // r13
  char v9; // si
  std::ostream *v10; // rax
  std::ostream *v11; // rbp
  _BYTE *v12; // r13
  char v13; // si
  std::ostream *v14; // rax
  __int64 (__fastcall *v16)(__int64, unsigned int); // rax
  __int64 (__fastcall *v17)(__int64, unsigned int); // rax
  _BYTE *v18; // r13
  char v19; // si
  std::ostream *v20; // rax
  __int64 (__fastcall *v21)(__int64, unsigned int); // rax
  __int64 (__fastcall *v22)(__int64, unsigned int); // rax
  __int64 (__fastcall *v23)(__int64, unsigned int); // rax
  std::ostream *v24; // rax
  void *v26; // [rsp+40h] [rbp-A8h] BYREF
  int v27[2]; // [rsp+48h] [rbp-A0h]
  int v28[2]; // [rsp+50h] [rbp-98h] BYREF
  void *v29; // [rsp+60h] [rbp-88h] BYREF
  int v30[2]; // [rsp+68h] [rbp-80h]
  int v31[2]; // [rsp+70h] [rbp-78h] BYREF
  void *v32; // [rsp+80h] [rbp-68h] BYREF
  __int64 v33; // [rsp+90h] [rbp-58h] BYREF
  int v34[2]; // [rsp+A8h] [rbp-40h]

  *(_QWORD *)v34 = __readfsqword(0x28u);
  LOBYTE(v28[0]) = 0;
  v26 = v28;
  *(_QWORD *)v27 = 0LL;
  v29 = v31;
  *(_QWORD *)v30 = 0LL;
  LOBYTE(v31[0]) = 0;
  std::__ostream_insert<char,std::char_traits<char>>();
  std::ostream::flush((std::ostream *)std::cout);
  v1 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
  if ( !v1 )
    std::__throw_bad_cast();
  if ( !v1[56] )
  {
    std::ctype<char>::_M_widen_init();
    v16 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v1 + 48LL);
    if ( v16 != std::ctype<char>::do_widen )
      ((void (__fastcall *)(_BYTE *, __int64, __int64))v16)(v1, 10LL, 10LL);
  }
  v2 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
  if ( (*((_BYTE *)v2 + *(_QWORD *)(*v2 - 24LL) + 32) & 5) == 0 )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v3 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v3 )
      std::__throw_bad_cast();
    if ( !v3[56] )
    {
      std::ctype<char>::_M_widen_init();
      v17 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v3 + 48LL);
      if ( v17 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v17)(v3, 10LL, 10LL);
    }
    v4 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v4 + *(_QWORD *)(*v4 - 24LL) + 32) & 5) == 0 )
    {
      if ( (unsigned __int8)sub_2D30((__int64 *)&v26) && (unsigned __int8)sub_2D30((__int64 *)&v29) )
      {
        v5 = *(void **)(a1 + 24);
        if ( v5 )
          free(v5);
        v6 = calloc(0x20uLL, 1uLL);
        *(_QWORD *)(a1 + 24) = v6;
        if ( v6 )
        {
          *(_DWORD *)a1 = 0;
          *(_DWORD *)(a1 + 4) = 0;
          *(_DWORD *)(a1 + 8) = 0;
          *(_QWORD *)(a1 + 16) = 0LL;
          std::__ostream_insert<char,std::char_traits<char>>();
          std::ostream::_M_insert<unsigned long>();
          std::__ostream_insert<char,std::char_traits<char>>();
          v7 = (std::ostream *)std::ostream::_M_insert<unsigned long>();
          v8 = *(_BYTE **)((char *)v7 + *(_QWORD *)(*(_QWORD *)v7 - 24LL) + 240);
          if ( !v8 )
            std::__throw_bad_cast();
          if ( v8[56] )
          {
            v9 = v8[67];
          }
          else
          {
            std::ctype<char>::_M_widen_init();
            v9 = 10;
            v23 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v8 + 48LL);
            if ( v23 != std::ctype<char>::do_widen )
              v9 = v23((__int64)v8, 10u);
          }
          v10 = (std::ostream *)std::ostream::put(v7, v9);
          std::ostream::flush(v10);
          std::__ostream_insert<char,std::char_traits<char>>();
          sub_3210((__int64)&v32, *(_QWORD *)(a1 + 24));
          v11 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
          v12 = *(_BYTE **)((char *)v11 + *(_QWORD *)(*(_QWORD *)v11 - 24LL) + 240);
          if ( !v12 )
            std::__throw_bad_cast();
          if ( v12[56] )
          {
            v13 = v12[67];
          }
          else
          {
            std::ctype<char>::_M_widen_init();
            v13 = 10;
            v22 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v12 + 48LL);
            if ( v22 != std::ctype<char>::do_widen )
              v13 = v22((__int64)v12, 10u);
          }
          v14 = (std::ostream *)std::ostream::put(v11, v13);
          std::ostream::flush(v14);
          if ( v32 != &v33 )
            operator delete(v32, v33 + 1);
        }
        else
        {
          v24 = (std::ostream *)std::operator<<<std::char_traits<char>>();
          sub_2CC0(v24);
        }
      }
      else
      {
        std::__ostream_insert<char,std::char_traits<char>>();
        v18 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
        if ( !v18 )
          std::__throw_bad_cast();
        if ( v18[56] )
        {
          v19 = v18[67];
        }
        else
        {
          std::ctype<char>::_M_widen_init();
          v19 = 10;
          v21 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v18 + 48LL);
          if ( v21 != std::ctype<char>::do_widen )
            v19 = v21((__int64)v18, 10u);
        }
        v20 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v19);
        std::ostream::flush(v20);
      }
    }
  }
  if ( v29 != v31 )
    operator delete(v29, *(_QWORD *)v31 + 1LL);
  if ( v26 != v28 )
    operator delete(v26, *(_QWORD *)v28 + 1LL);
  return *(_QWORD *)v34 - __readfsqword(0x28u);
}

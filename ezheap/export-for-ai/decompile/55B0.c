/*
 * func-name: patch_session_metadata
 * func-address: 0x55b0
 * callers: 0x8a60
 * callees: 0x2360, 0x2470, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2620, 0x2650, 0x2d30, 0x3210
 */

unsigned __int64 __fastcall patch_session_metadata(__int64 a1)
{
  _BYTE *v1; // r14
  _QWORD *v2; // rax
  _BYTE *v3; // r14
  _QWORD *v4; // rax
  _BYTE *v5; // r14
  _QWORD *v6; // rax
  _BYTE *v7; // r14
  char v8; // si
  std::ostream *v9; // rax
  __int64 (__fastcall *v11)(__int64, unsigned int); // rax
  __int64 (__fastcall *v12)(__int64, unsigned int); // rax
  __int64 (__fastcall *v13)(__int64, unsigned int); // rax
  __int64 (__fastcall *v14)(__int64, unsigned int); // rax
  char v15; // al
  _QWORD *v16; // r15
  std::ostream *v17; // r14
  _BYTE *v18; // r12
  char v19; // si
  std::ostream *v20; // rax
  __int64 (__fastcall *v21)(__int64, unsigned int); // rax
  _BYTE *v22; // r14
  __int64 (__fastcall *v23)(__int64, unsigned int); // rdx
  void *v24; // [rsp+40h] [rbp-C8h] BYREF
  int v25[2]; // [rsp+48h] [rbp-C0h]
  int v26[2]; // [rsp+50h] [rbp-B8h] BYREF
  void *v27; // [rsp+60h] [rbp-A8h] BYREF
  int v28[2]; // [rsp+68h] [rbp-A0h]
  int v29[2]; // [rsp+70h] [rbp-98h] BYREF
  void *v30[2]; // [rsp+80h] [rbp-88h] BYREF
  int v31[2]; // [rsp+90h] [rbp-78h] BYREF
  void *v32; // [rsp+A0h] [rbp-68h] BYREF
  __int64 v33; // [rsp+B0h] [rbp-58h] BYREF
  int v34[2]; // [rsp+C8h] [rbp-40h]

  *(_QWORD *)v34 = __readfsqword(0x28u);
  LOBYTE(v26[0]) = 0;
  v24 = v26;
  v27 = v29;
  *(_QWORD *)v25 = 0LL;
  *(_QWORD *)v28 = 0LL;
  LOBYTE(v29[0]) = 0;
  v30[0] = v31;
  v30[1] = 0LL;
  LOBYTE(v31[0]) = 0;
  std::__ostream_insert<char,std::char_traits<char>>();
  std::ostream::flush((std::ostream *)std::cout);
  v1 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
  if ( !v1 )
    std::__throw_bad_cast();
  if ( !v1[56] )
  {
    std::ctype<char>::_M_widen_init();
    v11 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v1 + 48LL);
    if ( v11 != std::ctype<char>::do_widen )
      ((void (__fastcall *)(_BYTE *, __int64, __int64))v11)(v1, 10LL, 10LL);
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
      v12 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v3 + 48LL);
      if ( v12 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v12)(v3, 10LL, 10LL);
    }
    v4 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v4 + *(_QWORD *)(*v4 - 24LL) + 32) & 5) == 0 )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      std::ostream::flush((std::ostream *)std::cout);
      v5 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
      if ( !v5 )
        std::__throw_bad_cast();
      if ( !v5[56] )
      {
        std::ctype<char>::_M_widen_init();
        v13 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v5 + 48LL);
        if ( v13 != std::ctype<char>::do_widen )
          ((void (__fastcall *)(_BYTE *, __int64, __int64))v13)(v5, 10LL, 10LL);
      }
      v6 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
      if ( (*((_BYTE *)v6 + *(_QWORD *)(*v6 - 24LL) + 32) & 5) == 0 )
      {
        if ( (unsigned __int8)sub_2D30((__int64 *)&v24)
          && (unsigned __int8)sub_2D30((__int64 *)&v27)
          && (unsigned __int8)sub_2D30((__int64 *)v30) )
        {
          v16 = *(_QWORD **)(a1 + 104);
          if ( !v16 )
          {
            std::__ostream_insert<char,std::char_traits<char>>();
            v22 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
            if ( !v22 )
              std::__throw_bad_cast();
            if ( v22[56] )
            {
              v15 = v22[67];
            }
            else
            {
              std::ctype<char>::_M_widen_init();
              v23 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v22 + 48LL);
              v15 = 10;
              if ( v23 != std::ctype<char>::do_widen )
                v15 = v23((__int64)v22, 10u);
            }
            goto LABEL_30;
          }
          if ( !*(_BYTE *)(a1 + 360) )
          {
            *v16 = 0LL;
            std::__ostream_insert<char,std::char_traits<char>>();
            sub_3210((__int64)&v32, (__int64)v16);
            v17 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
            v18 = *(_BYTE **)((char *)v17 + *(_QWORD *)(*(_QWORD *)v17 - 24LL) + 240);
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
            v20 = (std::ostream *)std::ostream::put(v17, v19);
            std::ostream::flush(v20);
            if ( v32 != &v33 )
              operator delete(v32, v33 + 1);
            goto LABEL_15;
          }
          std::__ostream_insert<char,std::char_traits<char>>();
          v7 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
          if ( !v7 )
            std::__throw_bad_cast();
        }
        else
        {
          std::__ostream_insert<char,std::char_traits<char>>();
          v7 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
          if ( !v7 )
            std::__throw_bad_cast();
        }
        if ( v7[56] )
        {
          v8 = v7[67];
LABEL_14:
          v9 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v8);
          std::ostream::flush(v9);
          goto LABEL_15;
        }
        std::ctype<char>::_M_widen_init();
        v8 = 10;
        v14 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v7 + 48LL);
        if ( v14 == std::ctype<char>::do_widen )
          goto LABEL_14;
        v15 = v14((__int64)v7, 10u);
LABEL_30:
        v8 = v15;
        goto LABEL_14;
      }
    }
  }
LABEL_15:
  if ( v30[0] != v31 )
    operator delete(v30[0], *(_QWORD *)v31 + 1LL);
  if ( v27 != v29 )
    operator delete(v27, *(_QWORD *)v29 + 1LL);
  if ( v24 != v26 )
    operator delete(v24, *(_QWORD *)v26 + 1LL);
  return *(_QWORD *)v34 - __readfsqword(0x28u);
}

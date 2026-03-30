/*
 * func-name: runtime_telemetry
 * func-address: 0x5d60
 * callers: 0x8a60
 * callees: 0x2360, 0x2400, 0x2460, 0x2470, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2610, 0x2620, 0x3210
 */

unsigned __int64 __fastcall runtime_telemetry(__int64 *a1)
{
  std::ostream *v1; // r12
  _BYTE *v2; // r13
  char v3; // si
  std::ostream *v4; // rax
  _BYTE *v5; // r12
  char v6; // si
  std::ostream *v7; // rax
  std::ostream *v8; // r12
  _BYTE *v9; // r13
  char v10; // si
  std::ostream *v11; // rax
  __int64 i; // r12
  char v13; // si
  std::ostream *v14; // rax
  _QWORD *v15; // rax
  _BYTE *v16; // r13
  __int64 (__fastcall *v17)(__int64, unsigned int); // rax
  std::ostream *v18; // r12
  _BYTE *v19; // r13
  char v20; // si
  unsigned __int64 v21; // r13
  std::ostream *v22; // rax
  char v23; // si
  std::ostream *v24; // rax
  std::ostream *v25; // r12
  _BYTE *v26; // r14
  __int64 (__fastcall *v27)(__int64, unsigned int); // rax
  __int64 v28; // rbp
  std::ostream *v29; // rbx
  _BYTE *v30; // rbp
  char v31; // si
  std::ostream *v32; // rax
  __int64 (__fastcall *v34)(__int64, unsigned int); // rax
  __int64 (__fastcall *v35)(__int64, unsigned int); // rax
  __int64 (__fastcall *v36)(__int64, unsigned int); // rax
  __int64 (__fastcall *v37)(__int64, unsigned int); // rax
  __int64 (__fastcall *v38)(__int64, unsigned int); // rax
  void *v39; // [rsp+20h] [rbp-A8h] BYREF
  __int64 v40; // [rsp+30h] [rbp-98h] BYREF
  void *v41; // [rsp+40h] [rbp-88h] BYREF
  _QWORD v42[2]; // [rsp+50h] [rbp-78h] BYREF
  void *v43; // [rsp+60h] [rbp-68h] BYREF
  _QWORD v44[3]; // [rsp+70h] [rbp-58h] BYREF
  __int64 v45; // [rsp+88h] [rbp-40h]

  v45 = __readfsqword(0x28u);
  std::__ostream_insert<char,std::char_traits<char>>();
  sub_3210((__int64)&v43, a1[3]);
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  std::ostream::_M_insert<unsigned long>();
  std::__ostream_insert<char,std::char_traits<char>>();
  v1 = (std::ostream *)std::ostream::_M_insert<unsigned long>();
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
    v36 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v2 + 48LL);
    if ( v36 != std::ctype<char>::do_widen )
      v3 = v36((__int64)v2, 10u);
  }
  v4 = (std::ostream *)std::ostream::put(v1, v3);
  std::ostream::flush(v4);
  if ( v43 != v44 )
    operator delete(v43, v44[0] + 1LL);
  std::__ostream_insert<char,std::char_traits<char>>();
  sub_3210((__int64)&v43, a1[4]);
  std::__ostream_insert<char,std::char_traits<char>>();
  if ( v43 != v44 )
    operator delete(v43, v44[0] + 1LL);
  if ( a1[4] )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::operator<<();
  }
  v5 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
  if ( !v5 )
    goto LABEL_65;
  if ( v5[56] )
  {
    v6 = v5[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v6 = 10;
    v35 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v5 + 48LL);
    if ( v35 != std::ctype<char>::do_widen )
      v6 = v35((__int64)v5, 10u);
  }
  v7 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v6);
  std::ostream::flush(v7);
  std::__ostream_insert<char,std::char_traits<char>>();
  sub_3210((__int64)&v41, (__int64)session_postproc_default);
  std::__ostream_insert<char,std::char_traits<char>>();
  std::__ostream_insert<char,std::char_traits<char>>();
  sub_3210((__int64)&v43, (__int64)audit_sink_print_flag);
  v8 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
  v9 = *(_BYTE **)((char *)v8 + *(_QWORD *)(*(_QWORD *)v8 - 24LL) + 240);
  if ( !v9 )
    std::__throw_bad_cast();
  if ( v9[56] )
  {
    v10 = v9[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v10 = 10;
    v37 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v9 + 48LL);
    if ( v37 != std::ctype<char>::do_widen )
      v10 = v37((__int64)v9, 10u);
  }
  v11 = (std::ostream *)std::ostream::put(v8, v10);
  std::ostream::flush(v11);
  if ( v43 != v44 )
    operator delete(v43, v44[0] + 1LL);
  if ( v41 != v42 )
    operator delete(v41, v42[0] + 1LL);
  for ( i = 0LL; i != 16; ++i )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::_M_insert<unsigned long>();
    std::__ostream_insert<char,std::char_traits<char>>();
    sub_3210((__int64)&v43, a1[i + 13]);
    std::__ostream_insert<char,std::char_traits<char>>();
    if ( v43 != v44 )
      operator delete(v43, v44[0] + 1LL);
    if ( a1[i + 13] )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      *(_DWORD *)((char *)&std::cout[3] + *(_QWORD *)(std::cout[0] - 24LL)) = *(_DWORD *)((_BYTE *)&std::cout[3]
                                                                                        + *(_QWORD *)(std::cout[0] - 24LL)) & 0xFFFFFFB5 | 8;
      std::ostream::_M_insert<unsigned long>();
      std::__ostream_insert<char,std::char_traits<char>>();
      v15 = (_QWORD *)std::ostream::_M_insert<unsigned long>();
      *(_DWORD *)((char *)v15 + *(_QWORD *)(*v15 - 24LL) + 24) = *(_DWORD *)((_BYTE *)v15 + *(_QWORD *)(*v15 - 24LL)
                                                                                          + 24) & 0xFFFFFFB5 | 2;
    }
    v16 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
    if ( !v16 )
      goto LABEL_65;
    if ( v16[56] )
    {
      v13 = v16[67];
    }
    else
    {
      std::ctype<char>::_M_widen_init();
      v13 = 10;
      v17 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v16 + 48LL);
      if ( v17 != std::ctype<char>::do_widen )
        v13 = v17((__int64)v16, 10u);
    }
    v14 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v13);
    std::ostream::flush(v14);
  }
  std::__ostream_insert<char,std::char_traits<char>>();
  v18 = (std::ostream *)std::ostream::_M_insert<unsigned long>();
  v19 = *(_BYTE **)((char *)v18 + *(_QWORD *)(*(_QWORD *)v18 - 24LL) + 240);
  if ( !v19 )
LABEL_65:
    std::__throw_bad_cast();
  if ( v19[56] )
  {
    v20 = v19[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v20 = 10;
    v38 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v19 + 48LL);
    if ( v38 != std::ctype<char>::do_widen )
      v20 = v38((__int64)v19, 10u);
  }
  v21 = 0LL;
  v22 = (std::ostream *)std::ostream::put(v18, v20);
  std::ostream::flush(v22);
  if ( a1[47] != a1[48] )
  {
    do
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      std::ostream::_M_insert<unsigned long>();
      std::__ostream_insert<char,std::char_traits<char>>();
      sub_3210((__int64)&v43, *(_QWORD *)(a1[47] + 8 * v21));
      v25 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
      v26 = *(_BYTE **)((char *)v25 + *(_QWORD *)(*(_QWORD *)v25 - 24LL) + 240);
      if ( !v26 )
        std::__throw_bad_cast();
      if ( v26[56] )
      {
        v23 = v26[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v23 = 10;
        v27 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v26 + 48LL);
        if ( v27 != std::ctype<char>::do_widen )
          v23 = v27((__int64)v26, 10u);
      }
      v24 = (std::ostream *)std::ostream::put(v25, v23);
      std::ostream::flush(v24);
      if ( v43 != v44 )
        operator delete(v43, v44[0] + 1LL);
      ++v21;
    }
    while ( v21 < (a1[48] - a1[47]) >> 3 );
  }
  v28 = a1[5];
  if ( v28 )
  {
    std::__ostream_insert<char,std::char_traits<char>>();
    sub_3210((__int64)&v39, v28);
    std::__ostream_insert<char,std::char_traits<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    sub_3210((__int64)&v41, *(_QWORD *)(v28 + 24));
    std::__ostream_insert<char,std::char_traits<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    sub_3210((__int64)&v43, *(_QWORD *)(v28 + 32));
    v29 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    strlen((const char *)(v28 + 40));
    std::__ostream_insert<char,std::char_traits<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    v30 = *(_BYTE **)((char *)v29 + *(_QWORD *)(*(_QWORD *)v29 - 24LL) + 240);
    if ( !v30 )
      std::__throw_bad_cast();
    if ( v30[56] )
    {
      v31 = v30[67];
    }
    else
    {
      std::ctype<char>::_M_widen_init();
      v31 = 10;
      v34 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v30 + 48LL);
      if ( v34 != std::ctype<char>::do_widen )
        v31 = v34((__int64)v30, 10u);
    }
    v32 = (std::ostream *)std::ostream::put(v29, v31);
    std::ostream::flush(v32);
    if ( v43 != v44 )
      operator delete(v43, v44[0] + 1LL);
    if ( v41 != v42 )
      operator delete(v41, v42[0] + 1LL);
    if ( v39 != &v40 )
      operator delete(v39, v40 + 1);
  }
  return v45 - __readfsqword(0x28u);
}

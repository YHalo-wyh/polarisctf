/*
 * func-name: provision_worker_profile
 * func-address: 0x6e90
 * callers: 0x8a60
 * callees: 0x2360, 0x2460, 0x2470, 0x24a0, 0x24b0, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x25d0, 0x2620, 0x2650, 0x2cc0, 0x2d30, 0x3210, 0x6d00
 */

// Allocates a 0x50 worker_profile. If a poisoned allocation lands on a task_desc, fields at +0x18/+0x20 overwrite handler and ctx.
__int64 __fastcall provision_worker_profile(_QWORD *a1)
{
  char *v1; // rax
  char *v2; // rbx
  _BYTE *v3; // r15
  _QWORD *v4; // rax
  _BYTE *v5; // r15
  _QWORD *v6; // rax
  _BYTE *v7; // r15
  _QWORD *v8; // rax
  _BYTE *v9; // r15
  _QWORD *v10; // rax
  _BYTE *v11; // r15
  _QWORD *v12; // rax
  _BYTE *v13; // r15
  _QWORD *v14; // rax
  _BYTE *v15; // r15
  _QWORD *v16; // rax
  _BYTE *v17; // rsi
  std::ostream *v18; // rax
  __int64 v19; // r8
  __int64 v20; // r9
  __int64 (__fastcall *v21)(__int64, unsigned int); // rax
  _BYTE *v23; // rbx
  char v24; // si
  std::ostream *v25; // rdi
  __int64 (__fastcall *v26)(__int64, unsigned int); // rax
  __int64 (__fastcall *v27)(__int64, unsigned int); // rax
  __int64 (__fastcall *v28)(__int64, unsigned int); // rax
  __int64 (__fastcall *v29)(__int64, unsigned int); // rax
  __int64 (__fastcall *v30)(__int64, unsigned int); // rax
  __int64 (__fastcall *v31)(__int64, unsigned int); // rax
  __int64 (__fastcall *v32)(__int64, unsigned int); // rax
  _BYTE *v33; // rbx
  char v34; // si
  std::ostream *v35; // rax
  __int64 (__fastcall *v36)(__int64, unsigned int); // rax
  int v37[2]; // [rsp+68h] [rbp-150h] BYREF
  void *v38[2]; // [rsp+70h] [rbp-148h] BYREF
  int v39[2]; // [rsp+80h] [rbp-138h] BYREF
  void *v40[2]; // [rsp+90h] [rbp-128h] BYREF
  int v41[2]; // [rsp+A0h] [rbp-118h] BYREF
  void *v42; // [rsp+B0h] [rbp-108h] BYREF
  int v43[2]; // [rsp+B8h] [rbp-100h]
  int v44[2]; // [rsp+C0h] [rbp-F8h] BYREF
  void *v45; // [rsp+D0h] [rbp-E8h] BYREF
  int v46[2]; // [rsp+D8h] [rbp-E0h]
  __int64 v47; // [rsp+E0h] [rbp-D8h] BYREF
  void *v48; // [rsp+F0h] [rbp-C8h] BYREF
  int v49[2]; // [rsp+F8h] [rbp-C0h]
  __int64 v50; // [rsp+100h] [rbp-B8h] BYREF
  void *v51; // [rsp+110h] [rbp-A8h] BYREF
  int v52[2]; // [rsp+118h] [rbp-A0h]
  int v53[2]; // [rsp+120h] [rbp-98h] BYREF
  void *v54; // [rsp+130h] [rbp-88h]
  int v55[2]; // [rsp+138h] [rbp-80h]
  int v56[2]; // [rsp+140h] [rbp-78h] BYREF
  int v57[2]; // [rsp+150h] [rbp-68h] BYREF
  int v58[2]; // [rsp+178h] [rbp-40h]

  *(_QWORD *)v58 = __readfsqword(0x28u);
  if ( a1[48] - a1[47] <= 0x1F8uLL )
  {
    v1 = (char *)operator new(0x50uLL);
    v38[1] = 0LL;
    *(_OWORD *)v1 = 0LL;
    v2 = v1;
    *(_QWORD *)v37 = v1;
    *((_OWORD *)v1 + 1) = 0LL;
    *((_OWORD *)v1 + 2) = 0LL;
    *((_OWORD *)v1 + 3) = 0LL;
    *((_OWORD *)v1 + 4) = 0LL;
    v38[0] = v39;
    v40[0] = v41;
    v42 = v44;
    v45 = &v47;
    v48 = &v50;
    v51 = v53;
    v54 = v56;
    LOBYTE(v39[0]) = 0;
    v40[1] = 0LL;
    LOBYTE(v41[0]) = 0;
    *(_QWORD *)v43 = 0LL;
    LOBYTE(v44[0]) = 0;
    *(_QWORD *)v46 = 0LL;
    LOBYTE(v47) = 0;
    *(_QWORD *)v49 = 0LL;
    LOBYTE(v50) = 0;
    *(_QWORD *)v52 = 0LL;
    LOBYTE(v53[0]) = 0;
    *(_QWORD *)v55 = 0LL;
    LOBYTE(v56[0]) = 0;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v3 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v3 )
      std::__throw_bad_cast();
    if ( !v3[56] )
    {
      std::ctype<char>::_M_widen_init();
      v21 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v3 + 48LL);
      if ( v21 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, void **, void **, __int64, __int64, void **, void **, void **))v21)(
          v3,
          10LL,
          &v42,
          v40,
          v19,
          v20,
          &v42,
          v40,
          v38);
    }
    v4 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v4 + *(_QWORD *)(*v4 - 24LL) + 32) & 5) != 0 )
      goto LABEL_34;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v5 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v5 )
      std::__throw_bad_cast();
    if ( !v5[56] )
    {
      std::ctype<char>::_M_widen_init();
      v26 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v5 + 48LL);
      if ( v26 != std::ctype<char>::do_widen )
        v26((__int64)v5, 10u);
    }
    v6 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v6 + *(_QWORD *)(*v6 - 24LL) + 32) & 5) != 0 )
      goto LABEL_34;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v7 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v7 )
      std::__throw_bad_cast();
    if ( !v7[56] )
    {
      std::ctype<char>::_M_widen_init();
      v28 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v7 + 48LL);
      if ( v28 != std::ctype<char>::do_widen )
        v28((__int64)v7, 10u);
    }
    v8 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v8 + *(_QWORD *)(*v8 - 24LL) + 32) & 5) != 0 )
      goto LABEL_34;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v9 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v9 )
      std::__throw_bad_cast();
    if ( !v9[56] )
    {
      std::ctype<char>::_M_widen_init();
      v29 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v9 + 48LL);
      if ( v29 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v29)(v9, 10LL, 10LL);
    }
    v10 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v10 + *(_QWORD *)(*v10 - 24LL) + 32) & 5) != 0 )
      goto LABEL_34;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v11 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v11 )
      std::__throw_bad_cast();
    if ( !v11[56] )
    {
      std::ctype<char>::_M_widen_init();
      v30 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v11 + 48LL);
      if ( v30 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v30)(v11, 10LL, 10LL);
    }
    v12 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v12 + *(_QWORD *)(*v12 - 24LL) + 32) & 5) != 0 )
      goto LABEL_34;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v13 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v13 )
      std::__throw_bad_cast();
    if ( !v13[56] )
    {
      std::ctype<char>::_M_widen_init();
      v31 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v13 + 48LL);
      if ( v31 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v31)(v13, 10LL, 10LL);
    }
    v14 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v14 + *(_QWORD *)(*v14 - 24LL) + 32) & 5) != 0 )
      goto LABEL_34;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v15 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v15 )
      std::__throw_bad_cast();
    if ( !v15[56] )
    {
      std::ctype<char>::_M_widen_init();
      v32 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v15 + 48LL);
      if ( v32 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v32)(v15, 10LL, 10LL);
    }
    v16 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v16 + *(_QWORD *)(*v16 - 24LL) + 32) & 5) != 0 )
    {
LABEL_34:
      operator delete(v2, 0x50uLL);
    }
    else
    {
      if ( (unsigned __int8)sub_2D30((__int64 *)v38)
        && (unsigned __int8)sub_2D30((__int64 *)v40)
        && (unsigned __int8)sub_2D30((__int64 *)&v42)
        && (unsigned __int8)sub_2D30((__int64 *)&v45)
        && (unsigned __int8)sub_2D30((__int64 *)&v48)
        && (unsigned __int8)sub_2D30((__int64 *)&v51) )
      {
        snprintf(v2 + 40, 0x20uLL, "%s", (const char *)v54);
        v17 = (_BYTE *)a1[48];
        if ( v17 == (_BYTE *)a1[49] )
        {
          sub_6D00((__int64)(a1 + 47), v17, (__int64 *)v37);
        }
        else
        {
          *(_QWORD *)v17 = v2;
          a1[48] = v17 + 8;
        }
        std::operator<<<std::char_traits<char>>();
        sub_3210((__int64)v57, (__int64)v2);
        std::__ostream_insert<char,std::char_traits<char>>();
        std::operator<<<std::char_traits<char>>();
        v18 = (std::ostream *)std::ostream::_M_insert<unsigned long>();
        sub_2CC0(v18);
        std::string::_M_dispose();
        std::string::_M_dispose();
        std::string::_M_dispose();
        std::string::_M_dispose();
        std::string::_M_dispose();
        std::string::_M_dispose();
        std::string::_M_dispose();
        std::string::_M_dispose();
        return *(_QWORD *)v58 - __readfsqword(0x28u);
      }
      operator delete(v2, 0x50uLL);
      std::__ostream_insert<char,std::char_traits<char>>();
      v33 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v33 )
        std::__throw_bad_cast();
      if ( v33[56] )
      {
        v34 = v33[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v34 = 10;
        v36 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v33 + 48LL);
        if ( v36 != std::ctype<char>::do_widen )
          v34 = v36((__int64)v33, 10u);
      }
      v35 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v34);
      std::ostream::flush(v35);
    }
    if ( v54 != v56 )
      operator delete(v54, *(_QWORD *)v56 + 1LL);
    if ( v51 != v53 )
      operator delete(v51, *(_QWORD *)v53 + 1LL);
    if ( v48 != &v50 )
      operator delete(v48, v50 + 1);
    if ( v45 != &v47 )
      operator delete(v45, v47 + 1);
    if ( v42 != v44 )
      operator delete(v42, *(_QWORD *)v44 + 1LL);
    if ( v40[0] != v41 )
      operator delete(v40[0], *(_QWORD *)v41 + 1LL);
    if ( v38[0] != v39 )
      operator delete(v38[0], *(_QWORD *)v39 + 1LL);
    return *(_QWORD *)v58 - __readfsqword(0x28u);
  }
  std::__ostream_insert<char,std::char_traits<char>>();
  v23 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
  if ( !v23 )
    std::__throw_bad_cast();
  if ( v23[56] )
  {
    v24 = v23[67];
  }
  else
  {
    std::ctype<char>::_M_widen_init();
    v24 = 10;
    v27 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v23 + 48LL);
    if ( v27 != std::ctype<char>::do_widen )
      v24 = v27((__int64)v23, 10u);
  }
  v25 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v24);
  return std::ostream::flush(v25);
}

/*
 * func-name: complete_batch_inference
 * func-address: 0x3910
 * callers: 0x8a60
 * callees: 0x2360, 0x2470, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2510, 0x2520, 0x2530, 0x2620, 0x2650, 0x2d30
 */

// Completes a session, frees its payload and session object, then only clears active[slot]. sessions[slot] remains stale: this is the UAF root.
unsigned __int64 __fastcall complete_batch_inference(__int64 a1)
{
  _BYTE *v2; // r14
  _QWORD *v3; // rax
  _BYTE *v4; // r12
  char v5; // si
  std::ostream *v6; // rax
  __int64 v8; // r14
  void *v9; // rdx
  void *v10; // rdi
  int v11; // ecx
  unsigned int v12; // eax
  __int64 v13; // rsi
  void (__fastcall *v14)(__int64); // rax
  char v15; // si
  std::ostream *v16; // rax
  __int64 (__fastcall *v17)(__int64, unsigned int); // rax
  __int64 (__fastcall *v18)(__int64, unsigned int); // rax
  __int64 (__fastcall *v19)(__int64, unsigned int); // rax
  _BYTE *v20; // [rsp+8h] [rbp-80h]
  void *v21[2]; // [rsp+20h] [rbp-68h] BYREF
  _QWORD v22[3]; // [rsp+30h] [rbp-58h] BYREF
  unsigned __int64 v23; // [rsp+48h] [rbp-40h]

  v23 = __readfsqword(0x28u);
  LOBYTE(v22[0]) = 0;
  v21[0] = v22;
  v21[1] = 0LL;
  std::__ostream_insert<char,std::char_traits<char>>();
  std::ostream::flush((std::ostream *)std::cout);
  v2 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
  if ( !v2 )
    std::__throw_bad_cast();
  if ( !v2[56] )
  {
    std::ctype<char>::_M_widen_init();
    v17 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v2 + 48LL);
    if ( v17 != std::ctype<char>::do_widen )
      ((void (__fastcall *)(_BYTE *, __int64, __int64))v17)(v2, 10LL, 10LL);
  }
  v3 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
  if ( (*((_BYTE *)v3 + *(_QWORD *)(*v3 - 24LL) + 32) & 5) == 0 )
  {
    if ( !(unsigned __int8)sub_2D30((__int64 *)v21) )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v4 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v4 )
        std::__throw_bad_cast();
LABEL_6:
      if ( v4[56] )
      {
        v5 = v4[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v5 = 10;
        v18 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v4 + 48LL);
        if ( v18 != std::ctype<char>::do_widen )
          v5 = v18((__int64)v4, 10u);
      }
      v6 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v5);
      std::ostream::flush(v6);
      goto LABEL_9;
    }
    if ( !*(_BYTE *)(a1 + 360) )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v4 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v4 )
        std::__throw_bad_cast();
      goto LABEL_6;
    }
    v8 = *(_QWORD *)(a1 + 104);
    if ( !v8 )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v4 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v4 )
        std::__throw_bad_cast();
      goto LABEL_6;
    }
    v9 = *(void **)(v8 + 8);
    v10 = v9;
    if ( v9 )
    {
      if ( *(_DWORD *)(v8 + 4) < 2u )
      {
        v14 = *(void (__fastcall **)(__int64))(v8 + 56);
        if ( !v14 )
        {
LABEL_28:
          free(v10);
          goto LABEL_29;
        }
      }
      else
      {
        v11 = 13;
        v12 = 1;
        while ( 1 )
        {
          v13 = v12++;
          *((_BYTE *)v9 + v13) ^= v11;
          v11 += 13;
          if ( v12 >= *(_DWORD *)(v8 + 4) )
            break;
          v9 = *(void **)(v8 + 8);
        }
        v14 = *(void (__fastcall **)(__int64))(v8 + 56);
        if ( !v14 )
          goto LABEL_27;
      }
    }
    else
    {
      v14 = *(void (__fastcall **)(__int64))(v8 + 56);
      if ( !v14 )
      {
LABEL_29:
        operator delete((void *)v8, 0x50uLL);
        *(_BYTE *)(a1 + 360) = 0;
        std::__ostream_insert<char,std::char_traits<char>>();
        v4 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
        if ( !v4 )
          std::__throw_bad_cast();
        goto LABEL_6;
      }
    }
    if ( v14 != session_postproc_clamp_negative
      && v14 != session_postproc_default
      && v14 != session_postproc_shift_right )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v20 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v20 )
        std::__throw_bad_cast();
      if ( v20[56] )
      {
        v15 = v20[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v15 = 10;
        v19 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v20 + 48LL);
        if ( v19 != std::ctype<char>::do_widen )
          v15 = v19((__int64)v20, 10u);
      }
      v16 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v15);
      std::ostream::flush(v16);
      v14 = session_postproc_default;
    }
    v14(v8);
LABEL_27:
    v10 = *(void **)(v8 + 8);
    if ( !v10 )
      goto LABEL_29;
    goto LABEL_28;
  }
LABEL_9:
  if ( v21[0] != v22 )
    operator delete(v21[0], v22[0] + 1LL);
  return v23 - __readfsqword(0x28u);
}

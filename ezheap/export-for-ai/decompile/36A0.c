/*
 * func-name: bootstrap_async_scheduler
 * func-address: 0x36a0
 * callers: 0x8a60
 * callees: 0x2360, 0x2440, 0x24a0, 0x24f0, 0x2500, 0x2530, 0x2550
 */

// Initializes queue control and allocates 8 heap-backed task descriptors of size 0x50; each descriptor gets a fixed runner handler.
__int64 __fastcall bootstrap_async_scheduler(__int64 a1)
{
  _BYTE *v1; // rbp
  char v2; // si
  std::ostream *v3; // rax
  __int64 (__fastcall *v5)(__int64, unsigned int); // rax
  char v6; // al
  char *v7; // rax
  unsigned __int64 v8; // r14
  _OWORD *v9; // rax
  _OWORD *v10; // rbx
  unsigned __int64 v11; // r15
  void *v12; // rcx
  __int64 v13; // rdx
  _BYTE *v14; // rbp
  __int64 (__fastcall *v15)(__int64, unsigned int); // rdx
  std::ostream *v16; // rax

  if ( *(_QWORD *)(a1 + 32) )
  {
LABEL_2:
    std::__ostream_insert<char,std::char_traits<char>>();
    v1 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
    if ( v1 )
    {
      if ( v1[56] )
      {
        v2 = v1[67];
LABEL_5:
        v3 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v2);
        return std::ostream::flush(v3);
      }
      std::ctype<char>::_M_widen_init();
      v2 = 10;
      v5 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v1 + 48LL);
      if ( v5 == std::ctype<char>::do_widen )
        goto LABEL_5;
      v6 = v5((__int64)v1, 10u);
LABEL_8:
      v2 = v6;
      goto LABEL_5;
    }
    goto LABEL_23;
  }
  v7 = (char *)malloc(0x38uLL);
  *(_QWORD *)(a1 + 32) = v7;
  if ( v7 )
  {
    v8 = 0LL;
    *(_OWORD *)(v7 + 26) = 0LL;
    *(_QWORD *)v7 = 0x49464F524745LL;
    *((_WORD *)v7 + 4) = 257;
    *(_OWORD *)(v7 + 10) = 0LL;
    *(_OWORD *)(v7 + 40) = 0LL;
    while ( 1 )
    {
      v9 = malloc(0x50uLL);
      v10 = v9;
      if ( !v9 )
        break;
      v11 = v8 + 1;
      v12 = task_runner_log_ack;
      v9[1] = 0LL;
      v9[2] = 0LL;
      v9[3] = 0LL;
      v9[4] = 0LL;
      v13 = v8 / 3 + (((0xAAAAAAAAAAAAAAABLL * (unsigned __int128)v8) >> 64) & 0xFFFFFFFFFFFFFFFELL);
      if ( v8 != v13 )
      {
        v12 = task_runner_mul_ack;
        if ( v8 - v13 == 1 )
          v12 = task_runner_xor_ack;
      }
      *((_QWORD *)v9 + 3) = v12;
      *(_QWORD *)v9 = v8;
      *((_QWORD *)v9 + 1) = v11;
      *((_QWORD *)v9 + 4) = v9;
      __snprintf_chk((__int64)v9 + 40, 32LL, 1LL, 32LL, "sqe-%zu");
      *(_QWORD *)(a1 + 8 * v11 + 32) = v10;
      if ( v8 == 7 )
        goto LABEL_2;
      ++v8;
    }
    std::__ostream_insert<char,std::char_traits<char>>();
    v14 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
    if ( v14 )
    {
      if ( v14[56] )
      {
        v6 = v14[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v15 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v14 + 48LL);
        v6 = 10;
        if ( v15 != std::ctype<char>::do_widen )
          v6 = v15((__int64)v14, 10u);
      }
      goto LABEL_8;
    }
LABEL_23:
    std::__throw_bad_cast();
  }
  v16 = (std::ostream *)std::operator<<<std::char_traits<char>>();
  return sub_2CC0(v16);
}

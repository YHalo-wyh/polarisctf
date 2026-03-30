/*
 * func-name: sub_7FB0
 * func-address: 0x7fb0
 * callers: 0x8a60
 * callees: 0x2360, 0x2380, 0x2460, 0x2470, 0x24b0, 0x24c0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x25c0, 0x2620, 0x2640, 0x2650, 0x2d30, 0x4200, 0x7e60
 */

__int64 __fastcall sub_7FB0(__int64 a1)
{
  _BYTE *v2; // rbp
  _QWORD *v3; // rax
  _BYTE *v4; // rbp
  _QWORD *v5; // rax
  unsigned __int8 *v6; // rbp
  unsigned __int8 v7; // r15
  _BYTE *v8; // rax
  unsigned __int64 v9; // rdx
  unsigned __int64 v10; // rbp
  _BYTE *v11; // r15
  _BYTE *v12; // rbp
  char v13; // si
  std::ostream *v14; // rax
  __int64 (__fastcall *v16)(__int64, unsigned int); // rax
  __int64 (__fastcall *v17)(__int64, unsigned int); // rax
  _BYTE *v18; // rbx
  char v19; // si
  std::ostream *v20; // rdi
  _BYTE *v21; // rbp
  char v22; // si
  std::ostream *v23; // rax
  __int64 (__fastcall *v24)(__int64, unsigned int); // rax
  __int64 (__fastcall *v25)(__int64, unsigned int); // rax
  __int64 v26; // rax
  _QWORD *v27; // rsi
  _BYTE *v28; // rcx
  void *v29; // rdi
  char v30; // al
  char v31; // al
  char v32; // al
  int v33; // edx
  char v34; // al
  unsigned __int64 v35; // rdx
  unsigned __int64 v36; // rax
  unsigned __int64 v37; // rdx
  unsigned __int64 v38; // rcx
  unsigned __int64 v39; // rbp
  char *v40; // rsi
  unsigned __int64 v41; // rdi
  std::ostream *v42; // rbp
  _BYTE *v43; // r12
  char v44; // si
  __int64 (__fastcall *v45)(__int64, unsigned int); // rax
  __int64 (__fastcall *v46)(__int64, unsigned int); // rax
  unsigned __int64 v47; // [rsp+10h] [rbp-108h]
  _BYTE *v48; // [rsp+10h] [rbp-108h]
  unsigned __int8 *v49; // [rsp+18h] [rbp-100h]
  unsigned __int64 v50; // [rsp+20h] [rbp-F8h]
  char v51; // [rsp+47h] [rbp-D1h] BYREF
  unsigned __int64 v52; // [rsp+48h] [rbp-D0h]
  __m128i v53; // [rsp+50h] [rbp-C8h] BYREF
  _BYTE *v54; // [rsp+60h] [rbp-B8h]
  void *v55[2]; // [rsp+70h] [rbp-A8h] BYREF
  _QWORD v56[2]; // [rsp+80h] [rbp-98h] BYREF
  void *v57; // [rsp+90h] [rbp-88h]
  __int64 v58; // [rsp+98h] [rbp-80h]
  _QWORD v59[2]; // [rsp+A0h] [rbp-78h] BYREF
  void *v60; // [rsp+B0h] [rbp-68h]
  unsigned __int64 v61; // [rsp+B8h] [rbp-60h]
  _QWORD v62[3]; // [rsp+C0h] [rbp-58h] BYREF
  unsigned __int64 v63; // [rsp+D8h] [rbp-40h]

  v63 = __readfsqword(0x28u);
  if ( *(_QWORD *)(a1 + 24) )
  {
    v55[0] = v56;
    v55[1] = 0LL;
    LOBYTE(v56[0]) = 0;
    v57 = v59;
    v58 = 0LL;
    LOBYTE(v59[0]) = 0;
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v2 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v2 )
      std::__throw_bad_cast();
    if ( !v2[56] )
    {
      std::ctype<char>::_M_widen_init();
      v16 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v2 + 48LL);
      if ( v16 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v16)(v2, 10LL, 10LL);
    }
    v3 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v3 + *(_QWORD *)(*v3 - 24LL) + 32) & 5) != 0 )
      goto LABEL_29;
    v52 = 0LL;
    if ( !(unsigned __int8)sub_2D30((__int64 *)v55) )
    {
      std::__ostream_insert<char,std::char_traits<char>>();
      v21 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
      if ( !v21 )
        std::__throw_bad_cast();
      if ( v21[56] )
      {
        v22 = v21[67];
      }
      else
      {
        std::ctype<char>::_M_widen_init();
        v22 = 10;
        v25 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v21 + 48LL);
        if ( v25 != std::ctype<char>::do_widen )
          v22 = v25((__int64)v21, 10u);
      }
      v23 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v22);
      std::ostream::flush(v23);
      goto LABEL_29;
    }
    std::__ostream_insert<char,std::char_traits<char>>();
    std::ostream::flush((std::ostream *)std::cout);
    v4 = *(_BYTE **)((char *)&std::cin[30] + *(_QWORD *)(std::cin[0] - 24));
    if ( !v4 )
      std::__throw_bad_cast();
    if ( !v4[56] )
    {
      std::ctype<char>::_M_widen_init();
      v17 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v4 + 48LL);
      if ( v17 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v17)(v4, 10LL, 10LL);
    }
    v5 = (_QWORD *)std::getline<char,std::char_traits<char>,std::allocator<char>>();
    if ( (*((_BYTE *)v5 + *(_QWORD *)(*v5 - 24LL) + 32) & 5) != 0 )
    {
LABEL_29:
      if ( v57 != v59 )
        operator delete(v57, v59[0] + 1LL);
      if ( v55[0] != v56 )
        operator delete(v55[0], v56[0] + 1LL);
      return v63 - __readfsqword(0x28u);
    }
    v60 = v62;
    v54 = 0LL;
    v61 = 0LL;
    LOBYTE(v62[0]) = 0;
    v53 = 0LL;
    std::string::reserve();
    v6 = (unsigned __int8 *)v57;
    v49 = (unsigned __int8 *)v57 + v58;
    v47 = v61;
    if ( v57 != (char *)v57 + v58 )
    {
      do
      {
        while ( 1 )
        {
          v7 = *v6;
          if ( !isspace(*v6) )
            break;
          if ( v49 == ++v6 )
            goto LABEL_18;
        }
        v50 = v47 + 1;
        v8 = v60;
        if ( v60 == v62 )
          v9 = 15LL;
        else
          v9 = v62[0];
        if ( v9 < v50 )
        {
          std::string::_M_mutate();
          v8 = v60;
        }
        ++v6;
        v8[v47] = v7;
        v61 = v47 + 1;
        *((_BYTE *)v60 + v50) = 0;
        v47 = v61;
      }
      while ( v49 != v6 );
    }
LABEL_18:
    v10 = v47 & 1;
    if ( (v47 & 1) == 0 && v47 >> 1 )
    {
      v26 = operator new(v47 >> 1);
      v27 = v60;
      v28 = (_BYTE *)v26;
      v54 = (_BYTE *)(v26 + (v47 >> 1));
      v53 = _mm_unpacklo_epi64((__m128i)(unsigned __int64)v26, (__m128i)(unsigned __int64)v26);
      v29 = v60;
      if ( v61 )
      {
        while ( 1 )
        {
          v32 = *((_BYTE *)v27 + v10);
          v29 = v27;
          if ( (unsigned __int8)(v32 - 48) <= 9u )
          {
            v33 = v32 - 48;
          }
          else if ( (unsigned __int8)(v32 - 97) <= 5u )
          {
            v33 = v32 - 87;
          }
          else
          {
            v33 = v32 - 55;
            if ( (unsigned __int8)(v32 - 65) >= 6u )
              v33 = -1;
          }
          v34 = *((_BYTE *)v27 + v10 + 1);
          if ( (unsigned __int8)(v34 - 48) > 9u )
          {
            if ( (unsigned __int8)(v34 - 97) <= 5u )
            {
              v30 = v34 - 87;
            }
            else
            {
              if ( (unsigned __int8)(v34 - 65) > 5u )
                goto LABEL_71;
              v30 = v34 - 55;
            }
          }
          else
          {
            v30 = v34 - 48;
          }
          if ( v33 == -1 )
          {
LABEL_71:
            v11 = (_BYTE *)v53.m128i_i64[0];
            if ( (_BYTE *)v53.m128i_i64[0] != v28 )
              v53.m128i_i64[1] = v53.m128i_i64[0];
            v28 = (_BYTE *)v53.m128i_i64[0];
            goto LABEL_74;
          }
          v31 = (16 * v33) | v30;
          v51 = v31;
          if ( v54 == v28 )
          {
            sub_7E60((__int64)&v53, v28, &v51);
            v28 = (_BYTE *)v53.m128i_i64[1];
          }
          else
          {
            *v28++ = v31;
            v53.m128i_i64[1] = (__int64)v28;
          }
          v10 += 2LL;
          if ( v10 >= v61 )
            break;
          v27 = v60;
        }
        v29 = v60;
        v11 = (_BYTE *)v53.m128i_i64[0];
        if ( v60 != v62 )
          goto LABEL_75;
      }
      else
      {
        v11 = (_BYTE *)v26;
LABEL_74:
        if ( v27 == v62 )
          goto LABEL_23;
LABEL_75:
        v48 = v28;
        operator delete(v29, v62[0] + 1LL);
        v28 = v48;
      }
      if ( v11 != v28 )
      {
        v35 = *(_QWORD *)(a1 + 16);
        v36 = v52;
        if ( v52 >= v35 )
        {
          std::__ostream_insert<char,std::char_traits<char>>();
          v12 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
          if ( !v12 )
            std::__throw_bad_cast();
        }
        else
        {
          v37 = v35 - v52;
          v38 = v28 - v11;
          if ( v52 <= 0x5F )
          {
            v39 = 96 - v52;
            if ( 96 - v52 > v37 )
              v39 = v37;
            if ( v39 > v38 )
              v39 = v38;
            v40 = &v11[-v52];
            v41 = v52 + v39;
            do
            {
              *(_BYTE *)(*(_QWORD *)(a1 + 24) + v36) = v40[v36];
              ++v36;
            }
            while ( v41 != v36 );
            std::__ostream_insert<char,std::char_traits<char>>();
            v42 = (std::ostream *)std::ostream::_M_insert<unsigned long>();
            std::__ostream_insert<char,std::char_traits<char>>();
            v43 = *(_BYTE **)((char *)v42 + *(_QWORD *)(*(_QWORD *)v42 - 24LL) + 240);
            if ( !v43 )
              std::__throw_bad_cast();
            if ( v43[56] )
            {
              v44 = v43[67];
            }
            else
            {
              std::ctype<char>::_M_widen_init();
              v44 = 10;
              v45 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v43 + 48LL);
              if ( v45 != std::ctype<char>::do_widen )
                v44 = v45((__int64)v43, 10u);
            }
            v14 = (std::ostream *)std::ostream::put(v42, v44);
            goto LABEL_27;
          }
          std::__ostream_insert<char,std::char_traits<char>>();
          v12 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
          if ( !v12 )
            std::__throw_bad_cast();
        }
LABEL_24:
        if ( v12[56] )
        {
          v13 = v12[67];
        }
        else
        {
          std::ctype<char>::_M_widen_init();
          v13 = 10;
          v46 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v12 + 48LL);
          if ( v46 != std::ctype<char>::do_widen )
            v13 = v46((__int64)v12, 10u);
        }
        v14 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v13);
LABEL_27:
        std::ostream::flush(v14);
        if ( v11 )
          operator delete(v11, v54 - v11);
        goto LABEL_29;
      }
    }
    else
    {
      if ( v60 != v62 )
        operator delete(v60, v62[0] + 1LL);
      v11 = 0LL;
    }
LABEL_23:
    std::__ostream_insert<char,std::char_traits<char>>();
    v12 = *(_BYTE **)((char *)&std::cout[30] + *(_QWORD *)(std::cout[0] - 24LL));
    if ( !v12 )
      std::__throw_bad_cast();
    goto LABEL_24;
  }
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
    v24 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v18 + 48LL);
    if ( v24 != std::ctype<char>::do_widen )
      v19 = v24((__int64)v18, 10u);
  }
  v20 = (std::ostream *)std::ostream::put((std::ostream *)std::cout, v19);
  return std::ostream::flush(v20);
}

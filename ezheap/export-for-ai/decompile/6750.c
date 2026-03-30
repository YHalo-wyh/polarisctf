/*
 * func-name: audit_sink_print_flag
 * func-address: 0x6750
 * callers: 0x5d60
 * callees: 0x2360, 0x2390, 0x23b0, 0x23c0, 0x23d0, 0x23e0, 0x2470, 0x2490, 0x24c0, 0x24d0, 0x24e0, 0x24f0, 0x2500, 0x2520, 0x2530, 0x2560, 0x2580, 0x25b0, 0x25f0, 0x2600, 0x2620, 0x2650, 0x2660
 */

// Win sink: opens /flag or ./flag and prints the first line. Intended control-flow hijack target.
unsigned __int64 audit_sink_print_flag()
{
  _BYTE *v0; // r14
  std::ostream *v1; // r14
  _BYTE *v2; // r15
  char v3; // si
  std::ostream *v4; // rax
  __int64 (__fastcall *v6)(__int64, unsigned int); // rax
  _BYTE *v7; // r15
  char v8; // si
  std::ostream *v9; // rax
  __int64 (__fastcall *v10)(__int64, unsigned int); // rax
  __int64 (__fastcall *v11)(__int64, unsigned int); // rax
  __int64 v12; // [rsp+20h] [rbp-258h]
  int *v13; // [rsp+30h] [rbp-248h]
  __int64 v14; // [rsp+38h] [rbp-240h]
  void *v15; // [rsp+40h] [rbp-238h]
  _BYTE v16[184]; // [rsp+78h] [rbp-200h] BYREF
  _QWORD v17[4]; // [rsp+130h] [rbp-148h] BYREF
  int v18; // [rsp+150h] [rbp-128h]
  __int64 v19; // [rsp+208h] [rbp-70h]
  __int16 v20; // [rsp+210h] [rbp-68h]
  __int128 v21; // [rsp+218h] [rbp-60h]
  __int128 v22; // [rsp+228h] [rbp-50h]
  unsigned __int64 v23; // [rsp+238h] [rbp-40h]

  v23 = __readfsqword(0x28u);
  std::ios_base::ios_base((std::ios_base *)v17);
  v20 = 0;
  v21 = 0LL;
  v22 = 0LL;
  v17[0] = &unk_C9A0;
  v19 = 0LL;
  v13 = &dword_0;
  *(int **)((char *)&v13 + MEMORY[0xFFFFFFFFFFFFFFE8]) = &dword_0;
  v14 = 0LL;
  std::ios::init();
  v13 = (int *)&unk_CA98;
  v17[0] = (char *)&unk_CA98 + 40;
  std::filebuf::basic_filebuf();
  std::ios::init();
  std::filebuf::open();
  std::ios::clear();
  if ( v18 && (std::ios::clear(), std::filebuf::open(), std::ios::clear(), v18) )
  {
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
    v13 = (int *)&unk_CA98;
    v17[0] = (char *)&unk_CA98 + 40;
    v15 = &unk_CB00;
    std::filebuf::close();
  }
  else
  {
    LOBYTE(v12) = 0;
    v0 = *(_BYTE **)&v16[*((_QWORD *)v13 - 3) + 168];
    if ( !v0 )
      std::__throw_bad_cast();
    if ( !v0[56] )
    {
      std::ctype<char>::_M_widen_init();
      v6 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v0 + 48LL);
      if ( v6 != std::ctype<char>::do_widen )
        ((void (__fastcall *)(_BYTE *, __int64, __int64))v6)(v0, 10LL, 10LL);
    }
    std::getline<char,std::char_traits<char>,std::allocator<char>>();
    std::__ostream_insert<char,std::char_traits<char>>();
    v1 = (std::ostream *)std::__ostream_insert<char,std::char_traits<char>>();
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
      v10 = *(__int64 (__fastcall **)(__int64, unsigned int))(*(_QWORD *)v2 + 48LL);
      if ( v10 != std::ctype<char>::do_widen )
        v3 = v10((__int64)v2, 10u);
    }
    v4 = (std::ostream *)std::ostream::put(v1, v3);
    std::ostream::flush(v4);
    v13 = (int *)&unk_CA98;
    v17[0] = (char *)&unk_CA98 + 40;
    v15 = &unk_CB00;
    std::filebuf::close();
  }
  std::__basic_file<char>::~__basic_file();
  v15 = &unk_C9C0;
  std::locale::~locale((std::locale *)v16);
  v13 = &dword_0;
  *(int **)((char *)&v13 + MEMORY[0xFFFFFFFFFFFFFFE8]) = &dword_0;
  v17[0] = &unk_C9A0;
  v14 = 0LL;
  std::ios_base::~ios_base((std::ios_base *)v17);
  return v23 - __readfsqword(0x28u);
}

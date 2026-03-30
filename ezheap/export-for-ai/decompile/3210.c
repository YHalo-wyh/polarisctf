/*
 * func-name: sub_3210
 * func-address: 0x3210
 * callers: 0x4230, 0x4870, 0x4d60, 0x55b0, 0x5d60, 0x6e90
 * callees: 0x23c0, 0x23d0, 0x2420, 0x2460, 0x24c0, 0x24e0, 0x24f0, 0x2560, 0x2580, 0x25a0, 0x2670
 */

__int64 __fastcall sub_3210(__int64 a1, __int64 a2)
{
  __m128i si128; // xmm2
  __int64 v4; // r8
  __m128i v5; // xmm3
  std::ios_base *v6; // rdi
  __m128 v8; // [rsp+0h] [rbp-1E8h] BYREF
  std::locale *v9[2]; // [rsp+10h] [rbp-1D8h] BYREF
  std::ios_base *v10; // [rsp+20h] [rbp-1C8h]
  __int64 v11; // [rsp+28h] [rbp-1C0h]
  _OWORD v12[3]; // [rsp+30h] [rbp-1B8h] BYREF
  __int128 v13; // [rsp+60h] [rbp-188h]
  _BYTE v14[8]; // [rsp+70h] [rbp-178h] BYREF
  int v15; // [rsp+78h] [rbp-170h]
  void *v16; // [rsp+80h] [rbp-168h]
  __int64 v17; // [rsp+88h] [rbp-160h]
  _QWORD v18[2]; // [rsp+90h] [rbp-158h] BYREF
  _QWORD v19[28]; // [rsp+A0h] [rbp-148h] BYREF
  __int16 v20; // [rsp+180h] [rbp-68h]
  __int128 v21; // [rsp+188h] [rbp-60h]
  __int128 v22; // [rsp+198h] [rbp-50h]
  unsigned __int64 v23; // [rsp+1A8h] [rbp-40h]

  v11 = a2;
  v23 = __readfsqword(0x28u);
  v10 = (std::ios_base *)v19;
  v8 = _mm_loadh_ps((const double *)&off_CC10);
  *(__m128 *)v9 = _mm_loadh_ps((const double *)&off_CC18);
  std::ios_base::ios_base((std::ios_base *)v19);
  v20 = 0;
  v21 = 0LL;
  v22 = 0LL;
  v19[0] = &unk_C9A0;
  *(_QWORD *)&v12[0] = &dword_0;
  v19[27] = 0LL;
  *(_QWORD *)((char *)v12 + MEMORY[0xFFFFFFFFFFFFFFE8]) = &dword_0;
  std::ios::init();
  si128 = _mm_load_si128((const __m128i *)v9);
  v9[0] = (std::locale *)v14;
  v19[0] = &unk_CA70;
  v12[0] = si128;
  memset(&v12[1], 0, 32);
  v13 = 0LL;
  std::locale::locale((std::locale *)v14);
  *((_QWORD *)&v12[0] + 1) = &unk_CBA0;
  v15 = 16;
  v16 = v18;
  v17 = 0LL;
  LOBYTE(v18[0]) = 0;
  std::ios::init();
  std::__ostream_insert<char,std::char_traits<char>>();
  *(_DWORD *)((char *)&v12[1] + *(_QWORD *)(*(_QWORD *)&v12[0] - 24LL) + 8) = *(_DWORD *)((_BYTE *)&v12[1]
                                                                                        + *(_QWORD *)(*(_QWORD *)&v12[0] - 24LL)
                                                                                        + 8) & 0xFFFFFFB5 | 8;
  std::ostream::_M_insert<unsigned long>();
  v4 = v13;
  *(_QWORD *)(a1 + 8) = 0LL;
  *(_QWORD *)a1 = a1 + 16;
  *(_BYTE *)(a1 + 16) = 0;
  if ( v4 )
    std::string::_M_replace();
  else
    std::string::_M_assign();
  v5 = _mm_load_si128((const __m128i *)&v8);
  v19[0] = &unk_CA70;
  v12[0] = v5;
  if ( v16 != v18 )
    operator delete(v16, v18[0] + 1LL);
  *((_QWORD *)&v12[0] + 1) = &unk_C9C0;
  std::locale::~locale(v9[0]);
  v6 = v10;
  *(_QWORD *)&v12[0] = &dword_0;
  *(_QWORD *)((char *)v12 + MEMORY[0xFFFFFFFFFFFFFFE8]) = &dword_0;
  v19[0] = &unk_C9A0;
  std::ios_base::~ios_base(v6);
  return a1;
}

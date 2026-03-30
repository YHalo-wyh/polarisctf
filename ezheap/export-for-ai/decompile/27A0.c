/*
 * func-name: main
 * func-address: 0x27a0
 * callers: 0x2b60
 * callees: 0x23f0, 0x24a0, 0x24b0, 0x24e0, 0x2cc0, 0x3570, 0x6720, 0x8a60
 */

__int64 __fastcall main(int a1, char **a2, char **a3)
{
  __int64 v3; // r9
  __int64 v4; // rax
  __int64 v5; // rcx
  _BYTE *v6; // rsi
  _QWORD *v7; // rdx
  __int64 v8; // rdi
  __int64 v9; // rdi
  __int64 *v10; // r11
  __int64 i; // r10
  __int64 v12; // rdx
  __int64 v13; // kr00_8
  unsigned __int64 v14; // rcx
  __int64 v15; // rdi
  unsigned __int64 v16; // rcx
  unsigned __int64 v17; // rdi
  unsigned __int64 v18; // rcx
  unsigned __int64 j; // rsi
  unsigned __int64 v20; // rax
  unsigned __int64 v21; // r11
  unsigned __int64 v22; // rax
  __int64 v23; // rax
  std::ostream *v25; // rax
  __int64 v26; // [rsp+8h] [rbp-1F0h]
  __int64 v27; // [rsp+18h] [rbp-1E0h]
  _BYTE v28[392]; // [rsp+20h] [rbp-1D8h] BYREF
  __int64 v29; // [rsp+1A8h] [rbp-50h]
  unsigned __int64 v30; // [rsp+1B8h] [rbp-40h]

  v30 = __readfsqword(0x28u);
  std::ios_base::sync_with_stdio(0LL, (bool)a2);
  v3 = 17LL;
  qword_D248 = 0LL;
  v27 = 0LL;
  do
  {
    v4 = 0LL;
    memset(v28, 0, 0x180uLL);
    v5 = 0LL;
    v6 = v28;
    v7 = v28;
    do
    {
      *v7 = v4;
      v8 = v4 + 256;
      ++v4;
      ++v7;
      v7[15] = v5;
      v5 += 10LL;
      v7[31] = v8;
    }
    while ( v4 != 16 );
    v9 = v3 - 16;
    do
    {
      v10 = (__int64 *)v28;
      for ( i = 0LL; i != 16; ++i )
      {
        v12 = v10[16];
        v26 = *v10 + i + v9;
        *v10 = v26;
        v10[16] = __ROL8__(v9 + v12, (unsigned __int8)i + 1);
        v13 = i;
        v10[32] ^= v26 << (v13 % 7);
        ++v10;
      }
      ++v9;
    }
    while ( v3 != v9 );
    v14 = 0x1337133713371337LL;
    do
    {
      v15 = *(_QWORD *)v6;
      v6 += 8;
      v16 = 0xBF58476D1CE4E5B9LL * (__ROL8__(v15 - 0x61C8864680B583EBLL, 13) ^ v14);
      v17 = (v16 >> 31) ^ v16 ^ __ROL8__(*((_QWORD *)v6 + 15) - 0x61C8864680B583EBLL, 13);
      v18 = 0xBF58476D1CE4E5B9LL
          * (((0xBF58476D1CE4E5B9LL * v17) >> 31) ^ (0xBF58476D1CE4E5B9LL * v17) ^ __ROL8__(
                                                                                     *((_QWORD *)v6 + 31)
                                                                                   - 0x61C8864680B583EBLL,
                                                                                     13));
      v14 = (v18 >> 31) ^ v18;
    }
    while ( &v28[128] != v6 );
    for ( j = 0LL; j != 35; ++j )
    {
      v21 = j - ((((0xCCCCCCCCCCCCCCCDLL * (unsigned __int128)j) >> 64) & 0xFFFFFFFFFFFFFFFCLL) + j / 5);
      v22 = j + v14;
      if ( v21 == 2 )
      {
        v20 = v22 ^ 0xEF372FE94F8473D5LL;
      }
      else if ( v21 <= 2 )
      {
        if ( v21 )
          v20 = v22 ^ 0xEF372FE94F83B3C1LL;
        else
          v20 = v22 ^ 0xEF372FE94F8293C6LL;
      }
      else if ( v21 == 3 )
      {
        v20 = v22 ^ 0xEF372FE94F8573E7LL;
      }
      else
      {
        v20 = v22 ^ 0xEF372FE94F86D3F2LL;
      }
      v14 = ((0xBF58476D1CE4E5B9LL * v20) >> 31) ^ (0xBF58476D1CE4E5B9LL * v20) ^ ((((0xBF58476D1CE4E5B9LL * v20) >> 31) ^ (0xBF58476D1CE4E5B9LL * v20)) >> 17);
    }
    ++v3;
    v27 ^= v14;
  }
  while ( v3 != 21 );
  if ( v27 == 3735928559LL )
  {
    v25 = (std::ostream *)std::operator<<<std::char_traits<char>>();
    sub_2CC0(v25);
  }
  v29 = 0LL;
  *(_QWORD *)v28 = 0LL;
  *(_DWORD *)&v28[8] = 0;
  memset(&v28[16], 0, 376);
  v23 = operator new(0x200uLL);
  v29 = v23 + 512;
  *(__m128i *)&v28[376] = _mm_unpacklo_epi64((__m128i)(unsigned __int64)v23, (__m128i)(unsigned __int64)v23);
  sub_8A60(v28);
  runtime_ctx_cleanup(v28);
  sub_6720(&v28[376]);
  return 0LL;
}
